//
//  buffer.c
//  PDScores
//
//  Created by Erin Mounts on 2/9/15.
//  Copyright (c) 2015 Sage Bionetworks. All rights reserved.
//

#include "buffer.h"
#include <string.h>

void buffer_overlap(double *out, double *in, uint64_t inSize, uint64_t framelen,
                    uint64_t overlap)
{
    uint64_t frameinc = framelen - overlap;
    uint64_t columns = (inSize + frameinc - 1) / frameinc;
    
    double *pOut = out;
    double *pIn = in;
    double *endOfIn = in + inSize;
    uint64_t outSize = columns * framelen;
    double *endOfOut = pOut + outSize;
    
    // MATLAB uses column-major order
    // pad the first buffer (column) with zeroes for overlap
    for (uint64_t i = 0; i < overlap; ++i) {
        *pOut++ = 0.0;
    }
    
    // fill in the columns
    while (pIn < endOfIn) {
        // put the new stuff in at the end of this column
        for (uint64_t i = 0; i < frameinc && pIn < endOfIn; ++i) {
            *pOut++ = *pIn++;
        }
        
        if  (pIn < endOfIn) {
            // copy the overlap to the start of the next column
            for (uint64_t i = 0; i < overlap; ++i) {
                *pOut = *(pOut - overlap);
                pOut++;
            }
        }
    }
    
    // pad the end of the last column with zeroes
    while (pOut < endOfOut) {
        *pOut++ = 0.0;
    }
}

void buffer_nooverlap(double *out, double *in, uint64_t inSize, uint64_t framelen)
{
    buffer_overlap(out, in, inSize, framelen, 0);
}