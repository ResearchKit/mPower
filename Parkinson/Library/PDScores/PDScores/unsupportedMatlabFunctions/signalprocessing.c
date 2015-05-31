//
//  signalprocessing.c
//  PDScores
//
//  Created by Erin Mounts on 2/9/15.
//  Copyright (c) 2015 Sage Bionetworks. All rights reserved.
//

#include "signalprocessing.h"
#include <Accelerate/Accelerate.h>
#include <string.h>
#include <stdlib.h>
#include <dispatch/dispatch.h>

#define vDSP_hann_window_is_faster 0
#define vvsin_is_faster 1
#define vvsin_is_way_faster 0

#if vvsin_is_faster || vvsin_is_way_faster
static size_t allocatedBufSize;
static double *n_over_N_minus_ones;
static double *sins;

static inline bool ensureBufferSize(size_t neededSize)
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        allocatedBufSize = 512;
        n_over_N_minus_ones = calloc(allocatedBufSize, sizeof(double));
        sins = calloc(allocatedBufSize, sizeof(double));
    });
    
    if (allocatedBufSize < neededSize) {
        while (allocatedBufSize < neededSize) {
            allocatedBufSize <<= 1;
        }
        free(n_over_N_minus_ones);
        n_over_N_minus_ones = calloc(allocatedBufSize, sizeof(double));
        free(sins);
        sins = calloc(allocatedBufSize, sizeof(double));
    }
    
    return (sins && n_over_N_minus_ones);
}

#endif

void hanning(double *outBuf, unsigned long windowSize)
{
#if vDSP_hann_window_is_faster
    unsigned long n = windowSize + 1;
    double tempBuf[n];
    vDSP_hann_windowD(tempBuf, n, vDSP_HANN_DENORM);
    memcpy(outBuf, tempBuf + 1, windowSize * sizeof(double));
#elif vvsin_is_faster
    // traditional 0.5(1 - cos(2πn/(N-1))) calculation of Hann window loses precision at small & large n where cos ~ 1
    // vDSP_hann_windowD(...) appears to use this calculation as it suffers this loss of precision
    // sin²(πn/(N-1)) is mathematically equivalent, but doesn't suffer this problem and requires fewer operations
    // to calculate so we'll do that instead
    double N_minus_one = windowSize + 1; // calc for window two samples bigger, but skip zeroes in first and last position
    double one_over_N_minus_one = 1.0 / N_minus_one;
    double *p = outBuf;
    int halfwin = (int)(windowSize + 1) / 2;
    
    ensureBufferSize(halfwin);
    
    double *pMid = p + halfwin;
    double *pEnd = p + windowSize;
    // calculate the first half of the window
    vDSP_vrampD(&one_over_N_minus_one, &one_over_N_minus_one, n_over_N_minus_ones, 1, halfwin);
    vvsinpi(sins, n_over_N_minus_ones, &halfwin);
    vDSP_vmulD(sins, 1, sins, 1, outBuf, 1, halfwin);
    // second half is the mirror of the first half so just copy what we already calculated
    p = pMid;
    double *pMirror = (windowSize % 2) ? p - 2 : p - 1;
    
    // can't seem to find a vector copy-in-reverse operation (cblas_dcopy doesn't work with -1 stride)
    while (p < pEnd) {
        *p++ = *pMirror--;
    }
#elif vvsin_is_way_faster
    double N_minus_one = windowSize + 1; // calc for window two samples bigger, but skip zeroes in first and last position
    double one_over_N_minus_one = 1.0 / N_minus_one;
    int samples = (int)windowSize;
    
    ensureBufferSize(samples);

    vDSP_vrampD(&one_over_N_minus_one, &one_over_N_minus_one, n_over_N_minus_ones, 1, samples);
    vvsinpi(sins, n_over_N_minus_ones, &samples);
    vDSP_vmulD(sins, 1, sins, 1, outBuf, 1, samples);
#else
    double N_minus_one = windowSize + 1; // calc for window two samples bigger, but skip zeroes in first and last position
    double pi_over_N_minus_one = M_PI / N_minus_one;
    double pi_times_n_over_N_minus_one = pi_over_N_minus_one; // start at n == 1
    double *p = outBuf;
    int halfwin = (int)(windowSize + 1) / 2;
    
    double *pMid = p + halfwin;
    double *pEnd = p + windowSize;
    // calculate the first half of the window
    while (p < pMid) {
        double sinx = sin(pi_times_n_over_N_minus_one);
        *p++ = sinx * sinx;
        pi_times_n_over_N_minus_one += pi_over_N_minus_one;
    }
    // second half is the mirror of the first half so just copy what we already calculated
    p = pMid;
    double *pMirror = (windowSize % 2) ? p - 2 : p - 1;
    
    // can't seem to find a vector copy-in-reverse operation (cblas_dcopy doesn't work with -1 stride)
    while (p < pEnd) {
        *p++ = *pMirror--;
    }
#endif
}

void spectrogram(creal_T *outFourierTransform, double *outFrequencies, double *outTimes, double *inSignal, unsigned long signalSize, double *window, unsigned long overlap, unsigned long windowSize, double samplingRate)
{
    unsigned long fftSize = windowSize;					// sample size
    unsigned long fftSizeOver2 = fftSize/2;
    unsigned long log2n = ceil(log2(fftSize));          // bins
    
    double *in_real = (double *) malloc(fftSize * sizeof(double));
    DOUBLE_COMPLEX_SPLIT split_data;
    split_data.realp = (double *) malloc(fftSizeOver2 * sizeof(double));
    split_data.imagp = (double *) malloc(fftSizeOver2 * sizeof(double));
    
    FFTSetupD fftSetup = vDSP_create_fftsetupD(log2n, FFT_RADIX2);
    
    unsigned long framestep = windowSize - overlap;
    unsigned long frames = (signalSize - overlap) / framestep;
    
    double frameDuration = (double)framestep / samplingRate;
    double frameTime = 0.0;
    
    double frequencyStep = samplingRate / fftSize;
    double frequencyForBin = 0.0; // DC
    for (unsigned long bin = 0; bin <= fftSizeOver2; ++bin) {
        outFrequencies[bin] = frequencyForBin;
        frequencyForBin += frequencyStep;
    }
    
    for (unsigned long frame = 0; frame < frames; ++frame) {
        double *frameSignal = inSignal + (frame * framestep);
        
        //multiply by window
        vDSP_vmulD(frameSignal, 1, window, 1, in_real, 1, fftSize);
        
        //convert to split complex format with evens in real and odds in imag
        vDSP_ctozD((DOUBLE_COMPLEX *) in_real, 2, &split_data, 1, fftSizeOver2);
        
        //calc fft
        vDSP_fft_zripD(fftSetup, &split_data, 1, log2n, FFT_FORWARD);
        
        // convert back to interleaved complex format
        vDSP_ztocD(&split_data, 1, (DOUBLE_COMPLEX *) in_real, 2, fftSizeOver2);
        
        // Divide all coefficients by 2 due to scaling
        // https://developer.apple.com/library/ios/documentation/Performance/Conceptual/vDSP_Programming_Guide/UsingFourierTransforms/UsingFourierTransforms.html#//apple_ref/doc/uid/TP40005147-CH202-15952
        double scaleFactor = 0.5;
        vDSP_vsmulD(in_real, 1, &scaleFactor, in_real, 1, fftSize);
        
        double nyquist = in_real[1]; // unpack Nyquist value packed into complex part alongside DC value in real part
        in_real[1] = 0;
        
        unsigned long colSize = fftSizeOver2 + 1;
        creal_T *outColStart = outFourierTransform + frame * colSize;
//        cblas_dcopy((int)fftSizeOver2, in_real, 2, outColStart, 1);
        cblas_zcopy((int)fftSizeOver2, in_real, 1, outColStart, 1);
        outColStart[fftSizeOver2].re = nyquist;
        outColStart[fftSizeOver2].im = 0.0;
//        outColStart[fftSizeOver2] = in_real[1]; // Nyquist value packed in complex part alongside DC value in real part
        
        outTimes[frame] = frameTime;
        frameTime += frameDuration;
    }
    
    vDSP_destroy_fftsetupD(fftSetup);
    free(split_data.imagp);
    free(split_data.realp);
    free(in_real);
}