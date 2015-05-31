/*
 * log2.c
 *
 * Code generation for function 'log2'
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
#include "log2.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_log2(const double x[2], double f[2])
{
  int k;
  for (k = 0; k < 2; k++) {
    f[k] = scalar_log2(x[k]);
  }
}

double scalar_log2(double x)
{
  double y;
  int eint;
  double fdbl;
  if (x == 0.0) {
    y = rtMinusInf;
  } else if (x < 0.0) {
    y = rtNaN;
  } else if ((!rtIsInf(x)) && (!rtIsNaN(x))) {
    if ((!rtIsInf(x)) && (!rtIsNaN(x))) {
      fdbl = frexp(x, &eint);
    } else {
      fdbl = x;
      eint = 0;
    }

    if (fdbl == 0.5) {
      y = (double)eint - 1.0;
    } else {
      y = log(fdbl) / 0.69314718055994529 + (double)eint;
    }
  } else {
    y = x;
  }

  return y;
}

/* End of code generation (log2.c) */
