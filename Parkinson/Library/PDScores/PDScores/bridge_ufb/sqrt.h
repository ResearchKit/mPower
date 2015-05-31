/*
 * sqrt.h
 *
 * Code generation for function 'sqrt'
 *
 */

#ifndef __SQRT_H__
#define __SQRT_H__

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
extern void b_sqrt(emxArray_real_T *x);
extern void c_sqrt(emxArray_real_T *x);
extern void d_sqrt(emxArray_real_T *x);

#endif

/* End of code generation (sqrt.h) */
