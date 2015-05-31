/*
 * polyfit.h
 *
 * Code generation for function 'polyfit'
 *
 */

#ifndef __POLYFIT_H__
#define __POLYFIT_H__

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
extern void b_polyfit(const emxArray_real_T *x, const emxArray_real_T *y, double
                      p[3]);
extern void polyfit(const emxArray_real_T *x, const emxArray_real_T *y, double
                    p[2]);

#endif

/* End of code generation (polyfit.h) */
