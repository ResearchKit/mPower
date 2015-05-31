/*
 * iqr.c
 *
 * Code generation for function 'iqr'
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
#include "iqr.h"
#include "bridge_ufb_emxutil.h"
#include "median.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
double iqr(const emxArray_real_T *x)
{
  double b_y[2];
  int i;
  emxArray_int32_T *idx;
  int nj;
  int k;
  double r;
  if (x->size[0] == 0) {
    for (i = 0; i < 2; i++) {
      b_y[i] = rtNaN;
    }
  } else {
    emxInit_int32_T(&idx, 1);
    eml_sort_idx(x, idx);
    nj = x->size[0];
    while ((nj > 0) && rtIsNaN(x->data[idx->data[nj - 1] - 1])) {
      nj--;
    }

    if (nj < 1) {
      for (i = 0; i < 2; i++) {
        b_y[i] = rtNaN;
      }
    } else if (nj == 1) {
      for (nj = 0; nj < 2; nj++) {
        b_y[nj] = x->data[idx->data[0] - 1];
      }
    } else {
      for (k = 0; k < 2; k++) {
        r = (25.0 + 50.0 * (double)k) / 100.0 * (double)nj;
        i = (int)rt_roundd_snf(r);
        if (nj <= i) {
          b_y[k] = x->data[idx->data[nj - 1] - 1];
        } else {
          r -= (double)i;
          b_y[k] = (0.5 - r) * x->data[idx->data[i - 1] - 1] + (0.5 + r) *
            x->data[idx->data[i] - 1];
        }
      }
    }

    emxFree_int32_T(&idx);
  }

  return b_y[1] - b_y[0];
}

/* End of code generation (iqr.c) */
