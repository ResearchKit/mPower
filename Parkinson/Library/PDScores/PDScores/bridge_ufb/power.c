/*
 * power.c
 *
 * Code generation for function 'power'
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
#include "power.h"
#include "bridge_ufb_emxutil.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int unnamed_idx_0;
  int k;
  unnamed_idx_0 = (unsigned int)a->size[0];
  k = y->size[0];
  y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < (int)unnamed_idx_0; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

void c_power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int uv1[2];
  int i11;
  int k;
  for (i11 = 0; i11 < 2; i11++) {
    uv1[i11] = (unsigned int)a->size[i11];
  }

  i11 = y->size[0] * y->size[1];
  y->size[0] = (int)uv1[0];
  y->size[1] = (int)uv1[1];
  emxEnsureCapacity((emxArray__common *)y, i11, (int)sizeof(double));
  i11 = (int)uv1[0] * (int)uv1[1];
  for (k = 0; k < i11; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

void d_power(const emxArray_real_T *a, double b, emxArray_real_T *y)
{
  unsigned int unnamed_idx_0;
  int k;
  unnamed_idx_0 = (unsigned int)a->size[0];
  k = y->size[0];
  y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < (int)unnamed_idx_0; k++) {
    y->data[k] = rt_powd_snf(a->data[k], b);
  }
}

void e_power(const emxArray_real_T *b, emxArray_real_T *y)
{
  unsigned int unnamed_idx_0;
  int k;
  unnamed_idx_0 = (unsigned int)b->size[0];
  k = y->size[0];
  y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < (int)unnamed_idx_0; k++) {
    y->data[k] = rt_powd_snf(2.0, b->data[k]);
  }
}

void f_power(const emxArray_real_T *b, emxArray_real_T *y)
{
  unsigned int uv3[2];
  int k;
  for (k = 0; k < 2; k++) {
    uv3[k] = (unsigned int)b->size[k];
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)uv3[1];
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  for (k = 0; k < (int)uv3[1]; k++) {
    y->data[k] = rt_powd_snf(2.0, b->data[k]);
  }
}

void power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int uv0[2];
  int i7;
  int k;
  for (i7 = 0; i7 < 2; i7++) {
    uv0[i7] = (unsigned int)a->size[i7];
  }

  i7 = y->size[0] * y->size[1];
  y->size[0] = (int)uv0[0];
  y->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)y, i7, (int)sizeof(double));
  i7 = (int)uv0[0] * 3;
  for (k = 0; k < i7; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

/* End of code generation (power.c) */
