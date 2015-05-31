/*
 * abs.c
 *
 * Code generation for function 'abs'
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
#include "abs.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int unnamed_idx_0;
  int k;
  unnamed_idx_0 = (unsigned int)x->size[0];
  k = y->size[0];
  y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < x->size[0]; k++) {
    y->data[k] = fabs(x->data[k]);
  }
}

void c_abs(const emxArray_creal_T *x, emxArray_real_T *y)
{
  unsigned int uv4[2];
  int i16;
  int k;
  for (i16 = 0; i16 < 2; i16++) {
    uv4[i16] = (unsigned int)x->size[i16];
  }

  i16 = y->size[0] * y->size[1];
  y->size[0] = (int)uv4[0];
  y->size[1] = (int)uv4[1];
  emxEnsureCapacity((emxArray__common *)y, i16, (int)sizeof(double));
  i16 = x->size[0] * x->size[1];
  for (k = 0; k < i16; k++) {
    y->data[k] = hypot(x->data[k].re, x->data[k].im);
  }
}

void d_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int uv5[2];
  int i20;
  int k;
  for (i20 = 0; i20 < 2; i20++) {
    uv5[i20] = (unsigned int)x->size[i20];
  }

  i20 = y->size[0] * y->size[1];
  y->size[0] = (int)uv5[0];
  y->size[1] = (int)uv5[1];
  emxEnsureCapacity((emxArray__common *)y, i20, (int)sizeof(double));
  i20 = x->size[0] * x->size[1];
  for (k = 0; k < i20; k++) {
    y->data[k] = fabs(x->data[k]);
  }
}

void e_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int uv7[2];
  int k;
  for (k = 0; k < 2; k++) {
    uv7[k] = (unsigned int)x->size[k];
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)uv7[1];
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < x->size[1]; k++) {
    y->data[k] = fabs(x->data[k]);
  }
}

/* End of code generation (abs.c) */
