/*
 * median.h
 *
 * Code generation for function 'median'
 *
 */

#ifndef __MEDIAN_H__
#define __MEDIAN_H__

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
extern void eml_sort_idx(const emxArray_real_T *x, emxArray_int32_T *idx);
extern double median(const emxArray_real_T *x);
extern double vectormedian(const emxArray_real_T *v);

#endif

/* End of code generation (median.h) */
