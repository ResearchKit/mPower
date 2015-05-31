/*
 * pwchcore.h
 *
 * Code generation for function 'pwchcore'
 *
 */

#ifndef __PWCHCORE_H__
#define __PWCHCORE_H__

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
extern void b_pwchcore(const emxArray_real_T *x, const emxArray_real_T *y, int
  yoffset, const emxArray_real_T *s, const emxArray_real_T *dx, const
  emxArray_real_T *divdif, emxArray_real_T *pp_breaks, emxArray_real_T *pp_coefs);
extern void pwchcore(const emxArray_real_T *x, const emxArray_real_T *y, int
                     yoffset, const emxArray_real_T *s, emxArray_real_T
                     *pp_breaks, emxArray_real_T *pp_coefs);

#endif

/* End of code generation (pwchcore.h) */
