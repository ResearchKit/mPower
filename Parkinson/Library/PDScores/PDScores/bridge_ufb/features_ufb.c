/*
 * features_ufb.c
 *
 * Code generation for function 'features_ufb'
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
double features_ufb(const emxArray_real_T *ftvec, const emxArray_real_T *wvec,
                    const emxArray_real_T *ilog, const emxArray_real_T *ftmin,
                    const emxArray_real_T *ftmax, double fbmin, double fbmax)
{
  emxArray_real_T *ftvecnorm;
  int b_ftvecnorm;
  int loop_ub;
  emxArray_real_T *r26;
  emxArray_real_T *b_ilog;
  double fb;
  emxInit_real_T(&ftvecnorm, 2);

  /*  Receives raw features, normalization parameters and feature weights, and */
  /*  computes user feedback value. */
  /*  Inputs: */
  /*   ftvec - feature vector (1xN) */
  /*   wvec  - weight vector (Nx1) */
  /*   ilog  - list of features to apply logarithm (variance stabilization) */
  /*   ftmin - feature normalization vector minimum (1xN) */
  /*   ftmax - feature normalization vector maximum (1xN) */
  /*   fbmin - user feedback minimum */
  /*   fbmax - user feedback maximum */
  /*  Outputs: */
  /*   fbnorm - normalized user feedback value */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  b_ftvecnorm = ftvecnorm->size[0] * ftvecnorm->size[1];
  ftvecnorm->size[0] = 1;
  ftvecnorm->size[1] = ftvec->size[1];
  emxEnsureCapacity((emxArray__common *)ftvecnorm, b_ftvecnorm, (int)sizeof
                    (double));
  loop_ub = ftvec->size[0] * ftvec->size[1];
  for (b_ftvecnorm = 0; b_ftvecnorm < loop_ub; b_ftvecnorm++) {
    ftvecnorm->data[b_ftvecnorm] = ftvec->data[b_ftvecnorm];
  }

  emxInit_real_T(&r26, 2);
  b_ftvecnorm = r26->size[0] * r26->size[1];
  r26->size[0] = 1;
  r26->size[1] = ilog->size[1];
  emxEnsureCapacity((emxArray__common *)r26, b_ftvecnorm, (int)sizeof(double));
  loop_ub = ilog->size[1];
  for (b_ftvecnorm = 0; b_ftvecnorm < loop_ub; b_ftvecnorm++) {
    r26->data[r26->size[0] * b_ftvecnorm] = ftvec->data[ftvec->size[0] * ((int)
      ilog->data[ilog->size[0] * b_ftvecnorm] - 1)];
  }

  b_emxInit_real_T(&b_ilog, 1);
  b_ftvecnorm = b_ilog->size[0];
  b_ilog->size[0] = ilog->size[1];
  emxEnsureCapacity((emxArray__common *)b_ilog, b_ftvecnorm, (int)sizeof(double));
  loop_ub = ilog->size[1];
  for (b_ftvecnorm = 0; b_ftvecnorm < loop_ub; b_ftvecnorm++) {
    b_ilog->data[b_ftvecnorm] = ilog->data[ilog->size[0] * b_ftvecnorm];
  }

  b_ftvecnorm = b_ilog->size[0];
  loop_ub = 0;
  emxFree_real_T(&b_ilog);
  while (loop_ub <= b_ftvecnorm - 1) {
    r26->data[loop_ub] = log10(r26->data[loop_ub]);
    loop_ub++;
  }

  loop_ub = r26->size[1];
  for (b_ftvecnorm = 0; b_ftvecnorm < loop_ub; b_ftvecnorm++) {
    ftvecnorm->data[ftvecnorm->size[0] * ((int)ilog->data[ilog->size[0] *
      b_ftvecnorm] - 1)] = r26->data[r26->size[0] * b_ftvecnorm];
  }

  emxFree_real_T(&r26);
  b_ftvecnorm = ftvecnorm->size[0] * ftvecnorm->size[1];
  ftvecnorm->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)ftvecnorm, b_ftvecnorm, (int)sizeof
                    (double));
  loop_ub = ftvecnorm->size[0];
  b_ftvecnorm = ftvecnorm->size[1];
  loop_ub *= b_ftvecnorm;
  for (b_ftvecnorm = 0; b_ftvecnorm < loop_ub; b_ftvecnorm++) {
    ftvecnorm->data[b_ftvecnorm] = 2.0 * ((ftvecnorm->data[b_ftvecnorm] -
      ftmin->data[b_ftvecnorm]) / (ftmax->data[b_ftvecnorm] - ftmin->
      data[b_ftvecnorm])) - 1.0;
  }

  /*  Projection of features onto 1st PC to create simple user feedback */
  if ((ftvecnorm->size[1] == 1) || (wvec->size[0] == 1)) {
    fb = 0.0;
    for (b_ftvecnorm = 0; b_ftvecnorm < ftvecnorm->size[1]; b_ftvecnorm++) {
      fb += ftvecnorm->data[ftvecnorm->size[0] * b_ftvecnorm] * wvec->
        data[b_ftvecnorm];
    }
  } else {
    fb = 0.0;
    for (b_ftvecnorm = 0; b_ftvecnorm < ftvecnorm->size[1]; b_ftvecnorm++) {
      fb += ftvecnorm->data[ftvecnorm->size[0] * b_ftvecnorm] * wvec->
        data[b_ftvecnorm];
    }
  }

  emxFree_real_T(&ftvecnorm);

  /*  Normalize and re-scale 0-100, clamp */
  return fmax(fmin((fb - fbmin) / (fbmax - fbmin) * 100.0, 100.0), 0.0);
}

/* End of code generation (features_ufb.c) */
