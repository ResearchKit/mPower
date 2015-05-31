/*
 * var.c
 *
 * Code generation for function 'var'
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
#include "var.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
double var(const emxArray_real_T *x)
{
  double y;
  int n;
  int d;
  int ix;
  double xbar;
  int k;
  double r;
  n = x->size[0] - 2;
  if (x->size[0] > 1) {
    d = x->size[0] - 1;
  } else {
    d = x->size[0];
  }

  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    ix = 0;
    xbar = x->data[0];
    for (k = 0; k <= n; k++) {
      ix++;
      xbar += x->data[ix];
    }

    xbar /= (double)x->size[0];
    ix = 0;
    r = x->data[0] - xbar;
    y = r * r;
    for (k = 0; k <= n; k++) {
      ix++;
      r = x->data[ix] - xbar;
      y += r * r;
    }

    y /= (double)d;
  }

  return y;
}

/* End of code generation (var.c) */
