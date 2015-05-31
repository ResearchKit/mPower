/*
 * flipud.c
 *
 * Code generation for function 'flipud'
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
#include "flipud.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void flipud(emxArray_real_T *x)
{
  int m;
  int n;
  int md2;
  int j;
  int i;
  int x_idx_0;
  double xtmp;
  int b_x_idx_0;
  m = x->size[0];
  n = x->size[1];
  md2 = x->size[0] >> 1;
  for (j = 0; j + 1 <= n; j++) {
    for (i = 1; i <= md2; i++) {
      x_idx_0 = x->size[0];
      xtmp = x->data[(i + x_idx_0 * j) - 1];
      x_idx_0 = x->size[0];
      b_x_idx_0 = x->size[0];
      x->data[(i + x_idx_0 * j) - 1] = x->data[(m - i) + b_x_idx_0 * j];
      x_idx_0 = x->size[0];
      x->data[(m - i) + x_idx_0 * j] = xtmp;
    }
  }
}

/* End of code generation (flipud.c) */
