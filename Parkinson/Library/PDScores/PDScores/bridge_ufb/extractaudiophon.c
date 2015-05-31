/*
 * extractaudiophon.c
 *
 * Code generation for function 'extractaudiophon'
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
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void extractaudiophon(const emxArray_real_T *rawaudio)
{
  emxArray_real_T *b_rawaudio;
  int i1;
  int loop_ub;
  int unusedExpr[2];
  b_emxInit_real_T(&b_rawaudio, 1);

  /*  Finds the best segment of a phonation recording. */
  /*  Inputs: */
  /*   rawaudio - mono floating-point, [-1,+1] normalized phonation recording */
  /*  Outputs: */
  /*   audiout - best phonation signal */
  /*   indout  - segment index of phonation signal */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  i1 = b_rawaudio->size[0];
  b_rawaudio->size[0] = rawaudio->size[0];
  emxEnsureCapacity((emxArray__common *)b_rawaudio, i1, (int)sizeof(double));
  loop_ub = rawaudio->size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    b_rawaudio->data[i1] = rawaudio->data[i1];
  }

  vadsplitphon(b_rawaudio, unusedExpr);
  emxFree_real_T(&b_rawaudio);
}

/* End of code generation (extractaudiophon.c) */
