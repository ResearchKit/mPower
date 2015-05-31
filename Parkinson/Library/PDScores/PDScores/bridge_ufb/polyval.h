/*
 * polyval.h
 *
 * Code generation for function 'polyval'
 *
 */

#ifndef __POLYVAL_H__
#define __POLYVAL_H__

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
extern void polyval(const double p[3], const emxArray_real_T *x, emxArray_real_T
                    *y);

#endif

/* End of code generation (polyval.h) */
