/*
 * combine_vector_elements.c
 *
 * Code generation for function 'combine_vector_elements'
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
#include "combine_vector_elements.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void combine_vector_elements(const emxArray_real_T *x, double y[3])
{
  int ixstart;
  int ix;
  int iy;
  int i;
  double s;
  if (x->size[0] == 0) {
    for (ixstart = 0; ixstart < 3; ixstart++) {
      y[ixstart] = 0.0;
    }
  } else {
    ix = -1;
    iy = -1;
    for (i = 0; i < 3; i++) {
      ixstart = ix + 1;
      ix++;
      s = x->data[ixstart];
      for (ixstart = 2; ixstart <= x->size[0]; ixstart++) {
        ix++;
        s += x->data[ix];
      }

      iy++;
      y[iy] = s;
    }
  }
}

/* End of code generation (combine_vector_elements.c) */
