/*
 * repmat.h
 *
 * Code generation for function 'repmat'
 *
 */

#ifndef __REPMAT_H__
#define __REPMAT_H__

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
extern void b_repmat(const emxArray_real_T *a, emxArray_real_T *b);
extern void c_repmat(const emxArray_real_T *a, double varargin_2,
                     emxArray_real_T *b);
extern void d_repmat(double varargin_1, double varargin_2, emxArray_real_T *b);
extern void e_repmat(const emxArray_real_T *a, double varargin_2,
                     emxArray_real_T *b);
extern void f_repmat(double varargin_1, emxArray_real_T *b);
extern void repmat(const double a[3], double varargin_1, emxArray_real_T *b);

#endif

/* End of code generation (repmat.h) */
