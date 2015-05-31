/*
 * rdivide.h
 *
 * Code generation for function 'rdivide'
 *
 */

#ifndef __RDIVIDE_H__
#define __RDIVIDE_H__

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
extern double b_rdivide(double x, double y);
extern void c_rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z);
extern void d_rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
                      emxArray_real_T *z);
extern void e_rdivide(double x, const double y[2], double z[2]);
extern void f_rdivide(double x, const emxArray_real_T *y, emxArray_real_T *z);
extern void g_rdivide(const emxArray_real_T *y, emxArray_real_T *z);
extern void rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z);

#endif

/* End of code generation (rdivide.h) */
