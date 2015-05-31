/*
 * diff.h
 *
 * Code generation for function 'diff'
 *
 */

#ifndef __DIFF_H__
#define __DIFF_H__

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
extern void b_diff(const emxArray_real_T *x, emxArray_real_T *y);
extern void diff(const emxArray_real_T *x, emxArray_real_T *y);

#endif

/* End of code generation (diff.h) */
