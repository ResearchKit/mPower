/*
 * lomb.h
 *
 * Code generation for function 'lomb'
 *
 */

#ifndef __LOMB_H__
#define __LOMB_H__

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
extern void b_lomb(const emxArray_real_T *t, const emxArray_real_T *h,
                   emxArray_real_T *f, emxArray_real_T *P, emxArray_real_T *prob);
extern void lomb(const emxArray_real_T *t, const emxArray_real_T *h, double ofac,
                 double hifac, emxArray_real_T *f, emxArray_real_T *P,
                 emxArray_real_T *prob);

#endif

/* End of code generation (lomb.h) */
