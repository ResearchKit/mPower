/*
 * interp1.h
 *
 * Code generation for function 'interp1'
 *
 */

#ifndef __INTERP1_H__
#define __INTERP1_H__

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
extern void b_interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
                      *varargin_2, const emxArray_real_T *varargin_3,
                      emxArray_real_T *Vq);
extern void interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
                    *varargin_2, const emxArray_real_T *varargin_3,
                    emxArray_real_T *Vq);

#endif

/* End of code generation (interp1.h) */
