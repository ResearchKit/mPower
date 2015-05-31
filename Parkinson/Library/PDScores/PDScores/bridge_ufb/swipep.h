/*
 * swipep.h
 *
 * Code generation for function 'swipep'
 *
 */

#ifndef __SWIPEP_H__
#define __SWIPEP_H__

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
extern void b_swipep(const emxArray_real_T *x, double fs, emxArray_real_T *p,
                     emxArray_real_T *t, emxArray_real_T *s);
extern void swipep(const emxArray_real_T *x, double fs, const double plim[2],
                   double dt, double dlog2p, double dERBs, double woverlap,
                   double sTHR, emxArray_real_T *p, emxArray_real_T *t,
                   emxArray_real_T *s);

#endif

/* End of code generation (swipep.h) */
