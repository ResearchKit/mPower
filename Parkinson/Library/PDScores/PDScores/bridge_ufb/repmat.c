/*
 * repmat.c
 *
 * Code generation for function 'repmat'
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
#include "repmat.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_repmat(const emxArray_real_T *a, emxArray_real_T *b)
{
  int nrows;
  int jtilecol;
  int ibtile;
  int k;
  nrows = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  b->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b, nrows, (int)sizeof(double));
  nrows = a->size[0];
  for (jtilecol = 0; jtilecol < 3; jtilecol++) {
    ibtile = jtilecol * nrows;
    for (k = 0; k + 1 <= nrows; k++) {
      b->data[ibtile + k] = a->data[k];
    }
  }
}

void c_repmat(const emxArray_real_T *a, double varargin_2, emxArray_real_T *b)
{
  int nrows;
  int jtilecol;
  int ibtile;
  int k;
  nrows = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  b->size[1] = (int)varargin_2;
  emxEnsureCapacity((emxArray__common *)b, nrows, (int)sizeof(double));
  nrows = a->size[0];
  if ((nrows == 0) || ((int)varargin_2 == 0)) {
  } else {
    nrows = a->size[0];
    for (jtilecol = 1; jtilecol <= (int)varargin_2; jtilecol++) {
      ibtile = (jtilecol - 1) * nrows;
      for (k = 1; k <= nrows; k++) {
        b->data[(ibtile + k) - 1] = a->data[k - 1];
      }
    }
  }
}

void d_repmat(double varargin_1, double varargin_2, emxArray_real_T *b)
{
  int i19;
  int loop_ub;
  i19 = b->size[0] * b->size[1];
  b->size[0] = (int)varargin_1;
  b->size[1] = (int)varargin_2;
  emxEnsureCapacity((emxArray__common *)b, i19, (int)sizeof(double));
  loop_ub = (int)varargin_1 * (int)varargin_2;
  for (i19 = 0; i19 < loop_ub; i19++) {
    b->data[i19] = rtNaN;
  }
}

void e_repmat(const emxArray_real_T *a, double varargin_2, emxArray_real_T *b)
{
  int outsize_idx_1;
  int nrows;
  int jtilecol;
  int ibtile;
  int jcol;
  int iacol;
  int ibmat;
  int k;
  outsize_idx_1 = a->size[1] * (int)varargin_2;
  nrows = b->size[0] * b->size[1];
  b->size[0] = a->size[0];
  b->size[1] = outsize_idx_1;
  emxEnsureCapacity((emxArray__common *)b, nrows, (int)sizeof(double));
  nrows = a->size[0];
  if ((nrows == 0) || (outsize_idx_1 == 0)) {
  } else {
    nrows = a->size[0];
    outsize_idx_1 = a->size[1];
    for (jtilecol = 1; jtilecol <= (int)varargin_2; jtilecol++) {
      ibtile = (jtilecol - 1) * (nrows * outsize_idx_1) - 1;
      for (jcol = 0; jcol + 1 <= outsize_idx_1; jcol++) {
        iacol = jcol * nrows;
        ibmat = ibtile + jcol * nrows;
        for (k = 1; k <= nrows; k++) {
          b->data[ibmat + k] = a->data[(iacol + k) - 1];
        }
      }
    }
  }
}

void f_repmat(double varargin_1, emxArray_real_T *b)
{
  int i21;
  int loop_ub;
  i21 = b->size[0];
  b->size[0] = (int)varargin_1;
  emxEnsureCapacity((emxArray__common *)b, i21, (int)sizeof(double));
  loop_ub = (int)varargin_1;
  for (i21 = 0; i21 < loop_ub; i21++) {
    b->data[i21] = rtNaN;
  }
}

void repmat(const double a[3], double varargin_1, emxArray_real_T *b)
{
  int jcol;
  int ibmat;
  int itilerow;
  jcol = b->size[0] * b->size[1];
  b->size[0] = (int)varargin_1;
  b->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b, jcol, (int)sizeof(double));
  if ((int)varargin_1 == 0) {
  } else {
    for (jcol = 0; jcol < 3; jcol++) {
      ibmat = jcol * (int)varargin_1;
      for (itilerow = 1; itilerow <= (int)varargin_1; itilerow++) {
        b->data[(ibmat + itilerow) - 1] = a[jcol];
      }
    }
  }
}

/* End of code generation (repmat.c) */
