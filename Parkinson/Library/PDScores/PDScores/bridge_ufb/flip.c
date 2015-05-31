/*
 * flip.c
 *
 * Code generation for function 'flip'
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
#include "flip.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void flip(emxArray_real_T *x)
{
  int vlen;
  int npages;
  int n;
  int nd2;
  int j;
  int offset;
  int k;
  double tmp;
  if ((!(x->size[1] == 0)) && (x->size[0] > 1)) {
    vlen = x->size[0];
    npages = x->size[1];
    n = x->size[0];
    nd2 = x->size[0] >> 1;
    for (j = 0; j < npages; j++) {
      offset = j * vlen;
      for (k = 1; k <= nd2; k++) {
        tmp = x->data[(offset + k) - 1];
        x->data[(offset + k) - 1] = x->data[(offset + n) - k];
        x->data[(offset + n) - k] = tmp;
      }
    }
  }
}

/* End of code generation (flip.c) */
