/*
 * spline.h
 *
 * Code generation for function 'spline'
 *
 */

#ifndef __SPLINE_H__
#define __SPLINE_H__

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
extern void spline(const emxArray_real_T *x, const emxArray_real_T *y,
                   emxArray_real_T *output_breaks, emxArray_real_T *output_coefs);

#endif

/* End of code generation (spline.h) */
