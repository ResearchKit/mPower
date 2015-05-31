/*
 * quantile.c
 *
 * Code generation for function 'quantile'
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
#include "quantile.h"
#include "bridge_ufb_emxutil.h"
#include "iqr.h"
#include "median.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
double quantile(const emxArray_real_T *x)
{
  double y;
  emxArray_int32_T *idx;
  int nj;
  double r;
  int i;
  if (x->size[0] == 0) {
    y = rtNaN;
  } else {
    emxInit_int32_T(&idx, 1);
    eml_sort_idx(x, idx);
    nj = x->size[0];
    while ((nj > 0) && rtIsNaN(x->data[idx->data[nj - 1] - 1])) {
      nj--;
    }

    if (nj < 1) {
      y = rtNaN;
    } else if (nj == 1) {
      y = x->data[idx->data[0] - 1];
    } else {
      r = 0.95 * (double)nj;
      i = (int)rt_roundd_snf(r);
      if (nj <= i) {
        y = x->data[idx->data[nj - 1] - 1];
      } else {
        r -= (double)i;
        y = (0.5 - r) * x->data[idx->data[i - 1] - 1] + (0.5 + r) * x->data
          [idx->data[i] - 1];
      }
    }

    emxFree_int32_T(&idx);
  }

  return y;
}

/* End of code generation (quantile.c) */
