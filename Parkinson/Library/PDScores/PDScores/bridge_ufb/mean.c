/*
 * mean.c
 *
 * Code generation for function 'mean'
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
#include "mean.h"
#include "bridge_ufb_emxutil.h"
#include "rdivide.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_mean(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int sz[2];
  int ixstart;
  emxArray_real_T *b_y;
  int k;
  int ix;
  int iy;
  int i;
  double s;
  for (ixstart = 0; ixstart < 2; ixstart++) {
    sz[ixstart] = (unsigned int)x->size[ixstart];
  }

  emxInit_real_T(&b_y, 2);
  ixstart = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = (int)sz[1];
  emxEnsureCapacity((emxArray__common *)b_y, ixstart, (int)sizeof(double));
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    ixstart = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, ixstart, (int)sizeof(double));
    ixstart = b_y->size[0] * b_y->size[1];
    b_y->size[1] = (int)sz[1];
    emxEnsureCapacity((emxArray__common *)b_y, ixstart, (int)sizeof(double));
    k = (int)sz[1];
    for (ixstart = 0; ixstart < k; ixstart++) {
      b_y->data[ixstart] = 0.0;
    }
  } else {
    ix = -1;
    iy = -1;
    for (i = 1; i <= x->size[1]; i++) {
      ixstart = ix + 1;
      ix++;
      s = x->data[ixstart];
      for (k = 2; k <= x->size[0]; k++) {
        ix++;
        s += x->data[ix];
      }

      iy++;
      b_y->data[iy] = s;
    }
  }

  c_rdivide(b_y, x->size[0], y);
  emxFree_real_T(&b_y);
}

double mean(const emxArray_real_T *x)
{
  double y;
  int k;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[0]; k++) {
      y += x->data[k - 1];
    }
  }

  y /= (double)x->size[0];
  return y;
}

/* End of code generation (mean.c) */
