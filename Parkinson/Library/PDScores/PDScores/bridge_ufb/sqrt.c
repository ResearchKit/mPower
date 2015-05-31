/*
 * sqrt.c
 *
 * Code generation for function 'sqrt'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "extractaudiophon.h"
#include "fastdfa.h"
#include "features_bga.h"
#include "features_bpa.h"
#include "features_bta.h"
#include "features_bvav2.h"
#include "features_ufb.h"
#include "lomb.h"
#include "mfcc.h"
#include "swipep.h"
#include "vadsplitphon.h"
#include "sqrt.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_sqrt(emxArray_real_T *x)
{
  int i30;
  int k;
  i30 = x->size[0];
  for (k = 0; k < i30; k++) {
    x->data[k] = sqrt(x->data[k]);
  }
}

void c_sqrt(emxArray_real_T *x)
{
  int i31;
  int k;
  i31 = x->size[0] * x->size[1];
  for (k = 0; k < i31; k++) {
    x->data[k] = sqrt(x->data[k]);
  }
}

void d_sqrt(emxArray_real_T *x)
{
  int i33;
  int k;
  i33 = x->size[1];
  for (k = 0; k < i33; k++) {
    x->data[k] = sqrt(x->data[k]);
  }
}

/* End of code generation (sqrt.c) */
