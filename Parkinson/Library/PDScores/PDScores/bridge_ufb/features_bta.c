/*
 * features_bta.c
 *
 * Code generation for function 'features_bta'
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
#include "iqr.h"
#include "median.h"
#include "diff.h"
#include "abs.h"
#include "mean.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void features_bta(const emxArray_real_T *tap, double ft[2])
{
  int i12;
  emxArray_real_T *t;
  int loop_ub;
  double tapdt;
  emxArray_real_T *tapx;
  emxArray_real_T *dx;
  emxArray_boolean_T *i;
  emxArray_int32_T *r12;
  double tapiqr;

  /*  Computes basic tapping test features. */
  /*  Inputs: */
  /*   tap - tapping data vector: tap(:,1) - time points, */
  /*         tap(:,2:3) - X,Y touch screen coordinates */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  /*  Output feature vector */
  for (i12 = 0; i12 < 2; i12++) {
    ft[i12] = rtNaN;
  }

  /*  Ignore zero-length inputs */
  if (tap->size[0] == 0) {
  } else {
    b_emxInit_real_T(&t, 1);

    /*  Calculate relative time */
    loop_ub = tap->size[0];
    tapdt = tap->data[0];
    i12 = t->size[0];
    t->size[0] = loop_ub;
    emxEnsureCapacity((emxArray__common *)t, i12, (int)sizeof(double));
    for (i12 = 0; i12 < loop_ub; i12++) {
      t->data[i12] = tap->data[i12] - tapdt;
    }

    b_emxInit_real_T(&tapx, 1);

    /*  X,Y offset */
    loop_ub = tap->size[0];
    i12 = tapx->size[0];
    tapx->size[0] = loop_ub;
    emxEnsureCapacity((emxArray__common *)tapx, i12, (int)sizeof(double));
    for (i12 = 0; i12 < loop_ub; i12++) {
      tapx->data[i12] = tap->data[i12 + tap->size[0]];
    }

    tapdt = mean(tapx);
    i12 = tapx->size[0];
    emxEnsureCapacity((emxArray__common *)tapx, i12, (int)sizeof(double));
    loop_ub = tapx->size[0];
    for (i12 = 0; i12 < loop_ub; i12++) {
      tapx->data[i12] -= tapdt;
    }

    b_emxInit_real_T(&dx, 1);
    emxInit_boolean_T(&i, 1);

    /*  Find left/right finger 'depress' events */
    diff(tapx, dx);
    b_abs(dx, tapx);
    i12 = i->size[0];
    i->size[0] = tapx->size[0];
    emxEnsureCapacity((emxArray__common *)i, i12, (int)sizeof(boolean_T));
    loop_ub = tapx->size[0];
    for (i12 = 0; i12 < loop_ub; i12++) {
      i->data[i12] = (tapx->data[i12] > 20.0);
    }

    emxInit_int32_T(&r12, 1);
    eml_li_find(i, r12);
    i12 = dx->size[0];
    dx->size[0] = r12->size[0];
    emxEnsureCapacity((emxArray__common *)dx, i12, (int)sizeof(double));
    loop_ub = r12->size[0];
    emxFree_boolean_T(&i);
    for (i12 = 0; i12 < loop_ub; i12++) {
      dx->data[i12] = t->data[r12->data[i12] - 1];
    }

    emxFree_int32_T(&r12);
    emxFree_real_T(&t);

    /*  Find depress event intervals */
    diff(dx, tapx);

    /*  Median and spread of tapping rate */
    emxFree_real_T(&dx);
    if (tapx->size[0] == 0) {
      tapdt = rtNaN;
    } else {
      tapdt = vectormedian(tapx);
    }

    tapiqr = iqr(tapx);

    /*  Output tapping test feature vector */
    ft[0] = tapdt;
    ft[1] = tapiqr;
    emxFree_real_T(&tapx);
  }
}

/* End of code generation (features_bta.c) */
