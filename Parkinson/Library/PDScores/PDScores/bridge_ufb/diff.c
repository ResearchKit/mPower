/*
 * diff.c
 *
 * Code generation for function 'diff'
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
#include "diff.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int orderForDim;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp1;
  double tmp2;
  if (x->size[1] == 0) {
    orderForDim = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
  } else {
    if (x->size[1] - 1 <= 1) {
      orderForDim = x->size[1] - 1;
    } else {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      orderForDim = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = 0;
      emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
    } else {
      orderForDim = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = x->size[1] - 1;
      emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
      orderForDim = x->size[1] - 1;
      if (!(orderForDim == 0)) {
        orderForDim = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= x->size[1]; m++) {
          tmp1 = x->data[orderForDim];
          tmp2 = work_data_idx_0;
          work_data_idx_0 = tmp1;
          tmp1 -= tmp2;
          orderForDim++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

void diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int orderForDim;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp1;
  double tmp2;
  if (x->size[0] == 0) {
    orderForDim = y->size[0];
    y->size[0] = 0;
    emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
  } else {
    if (x->size[0] - 1 <= 1) {
      orderForDim = x->size[0] - 1;
    } else {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      orderForDim = y->size[0];
      y->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
    } else {
      orderForDim = y->size[0];
      y->size[0] = x->size[0] - 1;
      emxEnsureCapacity((emxArray__common *)y, orderForDim, (int)sizeof(double));
      orderForDim = x->size[0] - 1;
      if (!(orderForDim == 0)) {
        orderForDim = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= x->size[0]; m++) {
          tmp1 = x->data[orderForDim];
          tmp2 = work_data_idx_0;
          work_data_idx_0 = tmp1;
          tmp1 -= tmp2;
          orderForDim++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

/* End of code generation (diff.c) */
