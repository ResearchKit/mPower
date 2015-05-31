/*
 * atan2.h
 *
 * Code generation for function 'atan2'
 *
 */

#ifndef __ATAN2_H__
#define __ATAN2_H__

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
extern void b_atan2(const emxArray_real_T *y, const emxArray_real_T *x,
                    emxArray_real_T *r);

#endif

/* End of code generation (atan2.h) */
