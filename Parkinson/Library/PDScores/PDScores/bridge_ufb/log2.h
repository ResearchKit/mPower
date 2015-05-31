/*
 * log2.h
 *
 * Code generation for function 'log2'
 *
 */

#ifndef __LOG2_H__
#define __LOG2_H__

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
extern void b_log2(const double x[2], double f[2]);
extern double scalar_log2(double x);

#endif

/* End of code generation (log2.h) */
