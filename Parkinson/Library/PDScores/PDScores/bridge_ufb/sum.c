/*
 * sum.c
 *
 * Code generation for function 'sum'
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
#include "sum.h"
#include "bridge_ufb_emxutil.h"
#include "combine_vector_elements.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_sum(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int sz[2];
  int vstride;
  int iy;
  int ixstart;
  int j;
  int ix;
  double s;
  int k;
  for (vstride = 0; vstride < 2; vstride++) {
    sz[vstride] = (unsigned int)x->size[vstride];
  }

  vstride = y->size[0];
  y->size[0] = (int)sz[0];
  emxEnsureCapacity((emxArray__common *)y, vstride, (int)sizeof(double));
  if (x->size[0] == 0) {
    vstride = y->size[0];
    y->size[0] = (int)sz[0];
    emxEnsureCapacity((emxArray__common *)y, vstride, (int)sizeof(double));
    iy = (int)sz[0];
    for (vstride = 0; vstride < iy; vstride++) {
      y->data[vstride] = 0.0;
    }
  } else {
    vstride = x->size[0];
    iy = -1;
    ixstart = -1;
    for (j = 1; j <= vstride; j++) {
      ixstart++;
      ix = ixstart;
      s = x->data[ixstart];
      for (k = 0; k < 2; k++) {
        ix += vstride;
        s += x->data[ix];
      }

      iy++;
      y->data[iy] = s;
    }
  }
}

void c_sum(const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int sz[2];
  int vstride;
  int iy;
  int ixstart;
  int j;
  int ix;
  double s;
  int k;
  for (vstride = 0; vstride < 2; vstride++) {
    sz[vstride] = (unsigned int)x->size[vstride];
  }

  vstride = y->size[0];
  y->size[0] = (int)sz[0];
  emxEnsureCapacity((emxArray__common *)y, vstride, (int)sizeof(double));
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    vstride = y->size[0];
    y->size[0] = (int)sz[0];
    emxEnsureCapacity((emxArray__common *)y, vstride, (int)sizeof(double));
    iy = (int)sz[0];
    for (vstride = 0; vstride < iy; vstride++) {
      y->data[vstride] = 0.0;
    }
  } else {
    vstride = x->size[0];
    iy = -1;
    ixstart = -1;
    for (j = 1; j <= vstride; j++) {
      ixstart++;
      ix = ixstart;
      s = x->data[ixstart];
      for (k = 2; k <= x->size[1]; k++) {
        ix += vstride;
        s += x->data[ix];
      }

      iy++;
      y->data[iy] = s;
    }
  }
}

void sum(const emxArray_real_T *x, double y[3])
{
  combine_vector_elements(x, y);
}

/* End of code generation (sum.c) */
