/*
 * fastdfa.h
 *
 * Code generation for function 'fastdfa'
 *
 */

#ifndef __FASTDFA_H__
#define __FASTDFA_H__

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
extern void fastdfa(const emxArray_real_T *x, double *alpha, emxArray_real_T
                    *intervals, emxArray_real_T *flucts);

#endif

/* End of code generation (fastdfa.h) */
