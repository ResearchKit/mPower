//
//  PDScores.m
//  PDScores
//
//  Created by Erin Mounts on 2/9/15.
//  Copyright (c) 2015 Sage Bionetworks. All rights reserved.
//

#import "PDScores.h"
#import "bridge_ufb_types.h"
#import "features_bga.h"
#import "features_bpa.h"
#import "features_bta.h"
#import "features_bvav2.h"
#import "features_ufb.h"
#import "bridge_ufb_initialize.h"
@import AudioToolbox;
@import CoreMotion;

const double fbmin = -2.691233945298616;
const double fbmax = 0.112505762143074;
double wvecData[25] = {
    0.286231494507457,
    0.261192327610184,
    0.098853812937446,
    -0.004458839281134,
    0.260731349136143,
    0.015095507428701,
    -0.182309796525234,
    0.000814580220081,
    0.066284176369154,
    0.035261446077445,
    0.025754011784195,
    0.008627620841593,
    -0.127762049854148,
    0.174356985048491,
    -0.052879096662678,
    0.401736293839382,
    0.405038065529931,
    0.212529121993104,
    -0.034258234293573,
    0.277275905057248,
    0.322225334501282,
    0.169523634856027,
    0.163302597072440,
    0.098002052991279,
    0.255354469250380
};

double ftminData[] = { 75.873430138619014, 0.909064396681576, 0.750104755980688, -1.215275709611168, 0.028427110782721  , -1.108948895519833, -1.741205757224090, -10.311033700183184, -17.729737676923012, 0.754335846764612, 0.686455477854417, 0.851058827586193, 0.722586392689788, 0.014355432088169, -3.824923125327304, 0.143257499992615, 0.047538333004923, 0.015628695534973, 0, 10.795359985168215, -1.909234199669284, -3.943415064253990, 0.711488941278325, -1.301029995663882, -2.000000000030019 };

double ftmaxData[] = { 466.9711215027887, 3.5755702286801, 2.2598212362848, 0.1496246072862, 0.9341546762825, 35.4322147307475, 16.9814320112968, 11.8234139704683, 11.5353327327018, 2.2795084122082, 2.0381298848445, 2.0520841815250, 1.9541007270693, 1.7499836685623, 2.1517890538311, 1.8984966684948, 2.9557149990000, 3.9238716785348, 2.1887075139316, 208.9349398410068, 0.1922690870262, 2.1382862219459, 1.5896368980157, 0.6780629049743, 0.7158362751650 };

// individual scores fall in narrowed ranges; we use these to renormalize them to ~0-100 (actually ~10-90 based on
// Max Little's original feature data, to give a little room for outliers; or to 100 if original scores went there with a
// continuous distribution)
const double phonationRange[] = { 56.0, 100.0 }; // observed: all between 60-100, 3% spike at 100
const double gaitRange[] = { 39.0, 93.0 }; // observed: all between 46-86, except for outliers clamped at 0 and 100
const double postureRange[] = { 69.0, 100.0 }; // observed: all between 72-100, 11% spike at 100
const double tappingRange[] = { 80.0, 100.0 }; // observed: all between 85-100, 2% spike at 100
const double kNormalizedMinimum = 10.0;

@implementation PDScores

+ (void)initialize
{
    bridge_ufb_initialize();
}

+ (double)normalizedScoreFromScore:(double)score range:(const double[2])range
{
    double newRange = (range[1] == 100.0) ? 90.0 : 80.0;
    double rangeScale = newRange / (range[1] - range[0]);
    double nscore = (score - range[0]) * rangeScale + kNormalizedMinimum;
    
    // keep clam(ped) and carry on (i.e. old 0 and 100 -> new 0 and 100)
    nscore = MAX(MIN(nscore, 100.0), 0.0);
    
    return nscore;
}

+ (double)scoreFromNormalizedScore:(double)nscore range:(const double[2])range
{
    // clamped scores is still clamped scores
    if (nscore == 0.0 || nscore == 100.0) {
        return nscore;
    }
    
    // otherwise convert from new back to original
    double originalRange = range[1] - range[0];
    double newRange = (range[1] == 100.0) ? 90.0 : 80.0;
    double originalScale = originalRange / newRange;
    double score = (nscore - kNormalizedMinimum) * originalScale + range[0];
    
    return score;
}

+ (double)scoreFromGaitTest:(NSArray *)gaitData
{
    // format the gait data for the MATLAB code
    emxArray_real_T gait;
    int sizeOfGait[2] = {(int)[gaitData count], 4};
    gait.size = sizeOfGait;
    gait.data = malloc(sizeof(double) * sizeOfGait[0] * sizeOfGait[1]);
    double *pT = gait.data;
    double *pX = pT + sizeOfGait[0];
    double *pY = pX + sizeOfGait[0];
    double *pZ = pY + sizeOfGait[0];
    for (int i = 0; i < sizeOfGait[0]; ++i) {
        NSDictionary *accel = [gaitData objectAtIndex:i];
        *pT++ = [[accel objectForKey:@"timestamp"] doubleValue];
        *pX++ = [[accel objectForKey:@"x"] doubleValue];
        *pY++ = [[accel objectForKey:@"y"] doubleValue];
        *pZ++ = [[accel objectForKey:@"z"] doubleValue];
    }
    
    // get the features array
    double ft[7];
    features_bga(&gait, ft);
    for (int i = 0; i < 7; ++i) {
        if (isnan(ft[i]))
            return NAN;
    }

    static emxArray_real_T ftvec;
    static int sizeOfFtvec[2] = {1, 7};
    static emxArray_real_T wvec;
    static int sizeOfWvec[2] = {7, 1};
    static emxArray_real_T ilog;
    static int sizeOfIlog[2] = {1, 1};
    static double ilogData[1] = {2};
    static emxArray_real_T ftmin;
    static int sizeOfFtmin[2] = {7, 1};
    static emxArray_real_T ftmax;
    static int sizeOfFtmax[2] = {7, 1};
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        ftvec.size = sizeOfFtvec;
        wvec.size = sizeOfWvec;
        wvec.data = wvecData + 13;
        ilog.size = sizeOfIlog;
        ilog.data = ilogData;
        ftmin.size = sizeOfFtmin;
        ftmin.data = ftminData + 13;
        ftmax.size = sizeOfFtmax;
        ftmax.data = ftmaxData + 13;
    });
    
    
    
    ftvec.data = ft;
    double rawScore = features_ufb(&ftvec, &wvec, &ilog, &ftmin, &ftmax, fbmin, fbmax);
    double score = [self normalizedScoreFromScore:rawScore range:gaitRange];
    
    return score;
}

+ (double)scoreFromPhonationTest:(NSURL *)phonationAudioFile
{
    OSStatus err = noErr;
    
    ExtAudioFileRef audiofile;
    err = ExtAudioFileOpenURL((__bridge CFURLRef)phonationAudioFile, &audiofile);
    if (err || !audiofile) {
        NSLog(@"Failed to open audio file %@, OSStatus error: %d", phonationAudioFile.absoluteString, (int)err);
        return NAN;
    }
    
    // get some info about the file's format.
    AudioStreamBasicDescription fileFormat;
    UInt32 size = sizeof(fileFormat);
    err = ExtAudioFileGetProperty(audiofile, kExtAudioFileProperty_FileDataFormat, &size, &fileFormat);
    if (err) {
        NSLog(@"OSStatus error getting audio file property FileDataFormat: %d", (int)err);
        return NAN;
    }

    // tell the ExtAudioFile API what format we want samples back in
    AudioStreamBasicDescription clientFormat;
    bzero(&clientFormat, sizeof(clientFormat));
    clientFormat.mChannelsPerFrame = fileFormat.mChannelsPerFrame;
    clientFormat.mBytesPerFrame = 4;
    clientFormat.mBytesPerPacket = clientFormat.mBytesPerFrame;
    clientFormat.mFramesPerPacket = 1;
    clientFormat.mBitsPerChannel = 32;
    clientFormat.mFormatID = kAudioFormatLinearPCM;
    clientFormat.mSampleRate = fileFormat.mSampleRate;
    clientFormat.mFormatFlags = kLinearPCMFormatFlagIsFloat | kAudioFormatFlagIsNonInterleaved;
    err = ExtAudioFileSetProperty(audiofile, kExtAudioFileProperty_ClientDataFormat, sizeof(clientFormat), &clientFormat);
    if (err) {
        NSLog(@"OSStatus error setting audio file property ClientDataFormat: %d", (int)err);
        return NAN;
    }
    
    // find out how many frames we need to read
    SInt64 numFrames = 0;
    size = sizeof(numFrames);
    err = ExtAudioFileGetProperty(audiofile, kExtAudioFileProperty_FileLengthFrames, &size, &numFrames);
    if (err) {
        NSLog(@"OSStatus error getting audio file property FileLengthFrames: %d", (int)err);
        return NAN;
    }
    
    // create the buffers for reading in data
    AudioBufferList *bufferList = malloc(sizeof(AudioBufferList) + sizeof(AudioBuffer) * (clientFormat.mChannelsPerFrame - 1));
    bufferList->mNumberBuffers = clientFormat.mChannelsPerFrame;
    for (int ii=0; ii < bufferList->mNumberBuffers; ++ii) {
        bufferList->mBuffers[ii].mDataByteSize = (UInt32)(sizeof(float) * numFrames);
        bufferList->mBuffers[ii].mNumberChannels = 1;
        bufferList->mBuffers[ii].mData = malloc(bufferList->mBuffers[ii].mDataByteSize);
    }
    
    // read in the data
    UInt32 rFrames = (UInt32)numFrames;
    err = ExtAudioFileRead(audiofile, &rFrames, bufferList);
    if (err) {
        NSLog(@"OSStatus error %d reading audio file %@", (int)err, phonationAudioFile.absoluteString);
        // release the AudioToolbox buffers
        for (int ii=0; ii < bufferList->mNumberBuffers; ++ii) {
            free(bufferList->mBuffers[ii].mData);
        }
        free(bufferList);
        return NAN;
    }
    
    // close the file
    err = ExtAudioFileDispose(audiofile);
    if (err) {
        NSLog(@"OSStatus error %d closing audio file %@", (int)err, phonationAudioFile.absoluteString);
    }
    
    // convert to emxArray_real_T for use with MATLAB converted code
    emxArray_real_T audio;
    int sizeOfAudio[2] = {(int)numFrames, 1};
    audio.size = sizeOfAudio;
    audio.numDimensions = 1;
    audio.canFreeData = true;
    audio.allocatedSize = (int)(sizeof(double) * numFrames);
    audio.data = malloc(audio.allocatedSize);
    float *pSrc = bufferList->mBuffers[0].mData;
    float *pSrcEnd = pSrc + numFrames;
    double *pDst = audio.data;
    while (pSrc < pSrcEnd) {
        *pDst++ = (double)*pSrc++;
    }
    
    // release the AudioToolbox buffers
    for (int ii=0; ii < bufferList->mNumberBuffers; ++ii) {
        free(bufferList->mBuffers[ii].mData);
    }
    free(bufferList);
    bufferList = NULL;
    
    // get the features array
    double ft[13];
    double srate = fileFormat.mSampleRate;
    features_bvav2(&audio, srate, ft);
    for (int i = 0; i < 13; ++i) {
        if (isnan(ft[i]))
            return NAN;
    }
    
    // clean up
    free(audio.data);
    
    // generate a score
    static emxArray_real_T ftvec;
    static int sizeOfFtvec[2] = {1, 13};
    static emxArray_real_T wvec;
    static int sizeOfWvec[2] = {13, 1};
    static emxArray_real_T ilog;
    static int sizeOfIlog[2] = {1, 7};
    static double ilogData[7] = {2, 3, 4, 10, 11, 12, 13};
    static emxArray_real_T ftmin;
    static int sizeOfFtmin[2] = {13, 1};
    static emxArray_real_T ftmax;
    static int sizeOfFtmax[2] = {13, 1};

    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        ftvec.size = sizeOfFtvec;
        wvec.size = sizeOfWvec;
        wvec.data = wvecData;
        ilog.size = sizeOfIlog;
        ilog.data = ilogData;
        ftmin.size = sizeOfFtmin;
        ftmin.data = ftminData;
        ftmax.size = sizeOfFtmax;
        ftmax.data = ftmaxData;
    });
    
    ftvec.data = ft;
    double rawScore = features_ufb(&ftvec, &wvec, &ilog, &ftmin, &ftmax, fbmin, fbmax);
    double score = [self normalizedScoreFromScore:rawScore range:phonationRange];

    return score;
}

+ (double)scoreFromPostureTest:(NSArray *)postureData
{
    // format the posture data for the MATLAB code
    emxArray_real_T posture;
    int sizeOfPosture[2] = {(int)[postureData count], 4};
    posture.size = sizeOfPosture;
    posture.data = malloc(sizeof(double) * sizeOfPosture[0] * sizeOfPosture[1]);
    double *pT = posture.data;
    double *pX = pT + sizeOfPosture[0];
    double *pY = pX + sizeOfPosture[0];
    double *pZ = pY + sizeOfPosture[0];
    for (int i = 0; i < sizeOfPosture[0]; ++i) {
        NSDictionary *accel = [postureData objectAtIndex:i];
        *pT++ = [[accel objectForKey:@"timestamp"] doubleValue];
        *pX++ = [[accel objectForKey:@"x"] doubleValue];
        *pY++ = [[accel objectForKey:@"y"] doubleValue];
        *pZ++ = [[accel objectForKey:@"z"] doubleValue];
    }
    
    // get the features array
    double ft[3];
    features_bpa(&posture, ft);
    for (int i = 0; i < 3; ++i) {
        if (isnan(ft[i]))
            return NAN;
    }

    static emxArray_real_T ftvec;
    static int sizeOfFtvec[2] = {1, 3};
    static emxArray_real_T wvec;
    static int sizeOfWvec[2] = {3, 1};
    static emxArray_real_T ilog;
    static int sizeOfIlog[2] = {1, 2};
    static double ilogData[2] = {1, 2};
    static emxArray_real_T ftmin;
    static int sizeOfFtmin[2] = {3, 1};
    static emxArray_real_T ftmax;
    static int sizeOfFtmax[2] = {3, 1};
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        ftvec.size = sizeOfFtvec;
        wvec.size = sizeOfWvec;
        wvec.data = wvecData + 20;
        ilog.size = sizeOfIlog;
        ilog.data = ilogData;
        ftmin.size = sizeOfFtmin;
        ftmin.data = ftminData + 20;
        ftmax.size = sizeOfFtmax;
        ftmax.data = ftmaxData + 20;
    });
    
    ftvec.data = ft;
    double rawScore = features_ufb(&ftvec, &wvec, &ilog, &ftmin, &ftmax, fbmin, fbmax);
    double score = [self normalizedScoreFromScore:rawScore range:postureRange];
    
    return score;
}

+ (double)scoreFromTappingTest:(NSArray *)tappingData
{
    // format the tapping data for the MATLAB code
    emxArray_real_T tapping;
    int sizeOfTapping[2] = {(int)[tappingData count], 3};
    tapping.size = sizeOfTapping;
    tapping.data = malloc(sizeof(double) * sizeOfTapping[0] * sizeOfTapping[1]);
    double *pT = tapping.data;
    double *pX = pT + sizeOfTapping[0];
    double *pY = pX + sizeOfTapping[0];
    for (int i = 0; i < sizeOfTapping[0]; ++i) {
        NSDictionary *tap = [tappingData objectAtIndex:i];
        NSString *tapCoord = [tap objectForKey:@"TapCoordinate"];
        NSCharacterSet *curlyBraces = [NSCharacterSet characterSetWithCharactersInString:@"{}"];
        tapCoord = [tapCoord stringByTrimmingCharactersInSet:curlyBraces];
        NSArray *tapCoords = [tapCoord componentsSeparatedByString:@", "];
        
        *pT++ = [[tap objectForKey:@"TapTimeStamp"] doubleValue];
        *pX++ = [[tapCoords objectAtIndex:0] doubleValue];
        *pY++ = [[tapCoords objectAtIndex:1] doubleValue];
    }
    
    // get the features array
    double ft[2];
    features_bta(&tapping, ft);
    for (int i = 0; i < 2; ++i) {
        if (isnan(ft[i]))
            return NAN;
    }
    
    static emxArray_real_T ftvec;
    static int sizeOfFtvec[2] = {1, 2};
    static emxArray_real_T wvec;
    static int sizeOfWvec[2] = {2, 1};
    static emxArray_real_T ilog;
    static int sizeOfIlog[2] = {1, 2};
    static double ilogData[2] = {1, 2};
    static emxArray_real_T ftmin;
    static int sizeOfFtmin[2] = {2, 1};
    static emxArray_real_T ftmax;
    static int sizeOfFtmax[2] = {2, 1};
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        ftvec.size = sizeOfFtvec;
        wvec.size = sizeOfWvec;
        wvec.data = wvecData + 23;
        ilog.size = sizeOfIlog;
        ilog.data = ilogData;
        ftmin.size = sizeOfFtmin;
        ftmin.data = ftminData + 23;
        ftmax.size = sizeOfFtmax;
        ftmax.data = ftmaxData + 23;
    });
    
    ftvec.data = ft;
    double rawScore = features_ufb(&ftvec, &wvec, &ilog, &ftmin, &ftmax, fbmin, fbmax);
    double score = [self normalizedScoreFromScore:rawScore range:tappingRange];
    
    return score;
}


double rawScoreFromScore(double score)
{
    static double max_minus_min = fbmax - fbmin;
    double rawScore = ((score / 100.0) * max_minus_min) + fbmin;
    return rawScore;
}

+ (double)scoreFromPhonationTest:(NSURL *)phonationAudioFile gaitTest:(NSArray *)gaitData postureTest:(NSArray *)postureData tappingTest:(NSArray *)tappingData
{
    return [self combinedScoreFromPhonationScore:[self scoreFromPhonationTest:phonationAudioFile]
                                       gaitScore:[self scoreFromGaitTest:gaitData]
                                    postureScore:[self scoreFromPostureTest:postureData]
                                    tappingScore:[self scoreFromTappingTest:tappingData]];
}

+ (double)combinedScoreFromPhonationScore:(double)phonationScore gaitScore:(double)gaitScore postureScore:(double)postureScore tappingScore:(double)tappingScore
{
    double rawPhon = rawScoreFromScore([self scoreFromNormalizedScore:phonationScore range:phonationRange]);
    double rawGait = rawScoreFromScore([self scoreFromNormalizedScore:gaitScore range:gaitRange]);
    double rawPosture = rawScoreFromScore([self scoreFromNormalizedScore:postureScore range:postureRange]);
    double rawTapping = rawScoreFromScore([self scoreFromNormalizedScore:tappingScore range:tappingRange]);
    
    double overallScore = 100.0 * (rawPhon + rawGait + rawPosture + rawTapping - fbmin) / (fbmax - fbmin);
    
    double normalizedScore = MAX(MIN(overallScore, 100.0), 0.0);
    
    return normalizedScore;
}

@end
