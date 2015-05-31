/*
 * sum.h
 *
 * Code generation for function 'sum'
 *
 */

#ifndef __SUM_H__
#define __SUM_H__

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
extern void b_sum(const emxArray_real_T *x, emxArray_real_T *y);
extern void c_sum(const emxArray_real_T *x, emxArray_real_T *y);
extern void sum(const emxArray_real_T *x, double y[3]);

#endif

/* End of code generation (sum.h) */
