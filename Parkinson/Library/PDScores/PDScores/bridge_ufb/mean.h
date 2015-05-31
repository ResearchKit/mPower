/*
 * mean.h
 *
 * Code generation for function 'mean'
 *
 */

#ifndef __MEAN_H__
#define __MEAN_H__

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
extern void b_mean(const emxArray_real_T *x, emxArray_real_T *y);
extern double mean(const emxArray_real_T *x);

#endif

/* End of code generation (mean.h) */
