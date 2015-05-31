/*
 * log10.c
 *
 * Code generation for function 'log10'
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
#include "log10.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_log10(emxArray_real_T *x)
{
  int i28;
  int k;
  i28 = x->size[0];
  for (k = 0; k < i28; k++) {
    x->data[k] = log10(x->data[k]);
  }
}

/* End of code generation (log10.c) */
