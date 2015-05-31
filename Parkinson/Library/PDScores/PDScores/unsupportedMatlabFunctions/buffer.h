//
//  buffer.h
//  PDScores
//
//  Created by Erin Mounts on 2/9/15.
//  Copyright (c) 2015 Sage Bionetworks. All rights reserved.
//

#ifndef __PDScores__buffer__
#define __PDScores__buffer__

#include <sys/types.h>

extern void buffer_overlap(double *out, double *in, uint64_t inSize, uint64_t framelen,
                           uint64_t overlap);
extern void buffer_nooverlap(double *out, double *in, uint64_t inSize, uint64_t framelen);

#endif /* defined(__PDScores__buffer__) */
