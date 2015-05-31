/*
 * rdivide.c
 *
 * Code generation for function 'rdivide'
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
#include "rdivide.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
double b_rdivide(double x, double y)
{
  return x / y;
}

void c_rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z)
{
  int i4;
  int loop_ub;
  i4 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)z, i4, (int)sizeof(double));
  loop_ub = x->size[0] * x->size[1];
  for (i4 = 0; i4 < loop_ub; i4++) {
    z->data[i4] = x->data[i4] / y;
  }
}

void d_rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
               emxArray_real_T *z)
{
  int i10;
  int loop_ub;
  i10 = z->size[0];
  z->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)z, i10, (int)sizeof(double));
  loop_ub = x->size[0];
  for (i10 = 0; i10 < loop_ub; i10++) {
    z->data[i10] = x->data[i10] / y->data[i10];
  }
}

void e_rdivide(double x, const double y[2], double z[2])
{
  int i14;
  for (i14 = 0; i14 < 2; i14++) {
    z[i14] = x / y[i14];
  }
}

void f_rdivide(double x, const emxArray_real_T *y, emxArray_real_T *z)
{
  int i15;
  int loop_ub;
  i15 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)z, i15, (int)sizeof(double));
  loop_ub = y->size[0] * y->size[1];
  for (i15 = 0; i15 < loop_ub; i15++) {
    z->data[i15] = x / y->data[i15];
  }
}

void g_rdivide(const emxArray_real_T *y, emxArray_real_T *z)
{
  int i18;
  int loop_ub;
  i18 = z->size[0];
  z->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)z, i18, (int)sizeof(double));
  loop_ub = y->size[0];
  for (i18 = 0; i18 < loop_ub; i18++) {
    z->data[i18] = 1.0 / y->data[i18];
  }
}

void rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z)
{
  int i3;
  int loop_ub;
  i3 = z->size[0];
  z->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)z, i3, (int)sizeof(double));
  loop_ub = x->size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    z->data[i3] = x->data[i3] / y;
  }
}

/* End of code generation (rdivide.c) */
