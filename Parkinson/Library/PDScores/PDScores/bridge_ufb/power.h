/*
 * power.h
 *
 * Code generation for function 'power'
 *
 */

#ifndef __POWER_H__
#define __POWER_H__

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
extern void b_power(const emxArray_real_T *a, emxArray_real_T *y);
extern void c_power(const emxArray_real_T *a, emxArray_real_T *y);
extern void d_power(const emxArray_real_T *a, double b, emxArray_real_T *y);
extern void e_power(const emxArray_real_T *b, emxArray_real_T *y);
extern void f_power(const emxArray_real_T *b, emxArray_real_T *y);
extern void power(const emxArray_real_T *a, emxArray_real_T *y);

#endif

/* End of code generation (power.h) */
