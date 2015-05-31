/*
 * mfcc.h
 *
 * Code generation for function 'mfcc'
 *
 */

#ifndef __MFCC_H__
#define __MFCC_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "bridge_ufb_types.h"

/* Function Declarations */
extern void mfcc(const emxArray_real_T *samples, emxArray_real_T *cepstra);

#endif

/* End of code generation (mfcc.h) */
