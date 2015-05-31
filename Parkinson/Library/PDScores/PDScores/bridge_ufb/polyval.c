/*
 * polyval.c
 *
 * Code generation for function 'polyval'
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
#include "polyval.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void polyval(const double p[3], const emxArray_real_T *x, emxArray_real_T *y)
{
  unsigned int uv6[2];
  int i24;
  int loop_ub;
  int k;
  for (i24 = 0; i24 < 2; i24++) {
    uv6[i24] = (unsigned int)x->size[i24];
  }

  i24 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = (int)uv6[1];
  emxEnsureCapacity((emxArray__common *)y, i24, (int)sizeof(double));
  if (!((int)uv6[1] == 0)) {
    i24 = y->size[0] * y->size[1];
    y->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)y, i24, (int)sizeof(double));
    i24 = y->size[0] * y->size[1];
    y->size[1] = (int)uv6[1];
    emxEnsureCapacity((emxArray__common *)y, i24, (int)sizeof(double));
    loop_ub = (int)uv6[1];
    for (i24 = 0; i24 < loop_ub; i24++) {
      y->data[i24] = p[0];
    }

    for (k = 0; k < 2; k++) {
      i24 = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = x->size[1];
      emxEnsureCapacity((emxArray__common *)y, i24, (int)sizeof(double));
      loop_ub = x->size[0] * x->size[1];
      for (i24 = 0; i24 < loop_ub; i24++) {
        y->data[i24] = x->data[i24] * y->data[i24] + p[k + 1];
      }
    }
  }
}

/* End of code generation (polyval.c) */
