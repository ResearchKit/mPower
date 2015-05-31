/*
 * round.c
 *
 * Code generation for function 'round'
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
#include "round.h"
#include "iqr.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_round(double x[2])
{
  int k;
  for (k = 0; k < 2; k++) {
    x[k] = rt_roundd_snf(x[k]);
  }
}

/* End of code generation (round.c) */
