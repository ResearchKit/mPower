/*
 * vadsplitphon.c
 *
 * Code generation for function 'vadsplitphon'
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
#include "rdivide.h"
#include "abs.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Declarations */
static int div_nzp_s32_floor(int numerator, int denominator);

/* Function Definitions */
static int div_nzp_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (numerator >= 0) {
    absNumerator = (unsigned int)numerator;
  } else {
    absNumerator = (unsigned int)-numerator;
  }

  if (denominator >= 0) {
    absDenominator = (unsigned int)denominator;
  } else {
    absDenominator = (unsigned int)-denominator;
  }

  quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
  tempAbsQuotient = absNumerator / absDenominator;
  if (quotientNeedsNegation) {
    absNumerator %= absDenominator;
    if (absNumerator > 0U) {
      tempAbsQuotient++;
    }
  }

  if (quotientNeedsNegation) {
    quotient = -(int)tempAbsQuotient;
  } else {
    quotient = (int)tempAbsQuotient;
  }

  return quotient;
}

void vadsplitphon(emxArray_real_T *x, int i_size[2])
{
  emxArray_real_T *varargin_1;
  int ixstart;
  int n;
  double buflen;
  int ix;
  boolean_T exitg1;
  emxArray_real_T *b_x;
  emxArray_real_T *X;
  int i2;
  emxArray_real_T *b_X;
  int i;
  int sz[2];
  emxArray_real_T *y;
  emxArray_real_T *b_y;
  emxArray_real_T *amp;
  emxArray_real_T *r0;
  b_emxInit_real_T(&varargin_1, 1);

  /*  Segment a phonation recording, based on an attack-sustain-release state */
  /*  machine model. */
  /*  Inputs: */
  /*   x - mono floating-point, [-1,+1] normalized phonation recording */
  /*  Outputs: */
  /*   y - phonation segments */
  /*   i - sample index of start of each segment */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  /*  Normalize audio input */
  b_abs(x, varargin_1);
  ixstart = 1;
  n = varargin_1->size[0];
  buflen = varargin_1->data[0];
  if (varargin_1->size[0] > 1) {
    if (rtIsNaN(varargin_1->data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(varargin_1->data[ix - 1])) {
          buflen = varargin_1->data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < varargin_1->size[0]) {
      while (ixstart + 1 <= n) {
        if (varargin_1->data[ixstart] > buflen) {
          buflen = varargin_1->data[ixstart];
        }

        ixstart++;
      }
    }
  }

  b_emxInit_real_T(&b_x, 1);
  ix = b_x->size[0];
  b_x->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)b_x, ix, (int)sizeof(double));
  n = x->size[0];
  for (ix = 0; ix < n; ix++) {
    b_x->data[ix] = x->data[ix];
  }

  emxInit_real_T(&X, 2);
  rdivide(b_x, buflen, x);

  /*  Sample rate */
  /*  Frame duration (sec) */
  /*  Frame change duration (sec) */
  /*  Post-voice activity silence dur (sec) */
  /*  Voice threshold */
  /*  Mean absolute amplitude */
  /*  Ignore first few incomplete frames */
  buflen = (((double)x->size[0] + 441.0) - 1.0) / 441.0;
  ix = X->size[0] * X->size[1];
  X->size[0] = 1764;
  X->size[1] = (int)buflen;
  emxEnsureCapacity((emxArray__common *)X, ix, (int)sizeof(double));
  n = 1764 * (int)buflen;
  emxFree_real_T(&b_x);
  for (ix = 0; ix < n; ix++) {
    X->data[ix] = 0.0;
  }

  ix = varargin_1->size[0];
  varargin_1->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)varargin_1, ix, (int)sizeof(double));
  n = x->size[0];
  for (ix = 0; ix < n; ix++) {
    varargin_1->data[ix] = x->data[ix];
  }

  buffer_overlap(&X->data[0], &varargin_1->data[0], (double)x->size[0], 1764.0,
                 1323.0);
  emxFree_real_T(&varargin_1);
  if (4 > X->size[1]) {
    ix = 0;
    i2 = 0;
  } else {
    ix = 3;
    i2 = X->size[1];
  }

  emxInit_real_T(&b_X, 2);
  ixstart = b_X->size[0] * b_X->size[1];
  b_X->size[0] = 1764;
  b_X->size[1] = i2 - ix;
  emxEnsureCapacity((emxArray__common *)b_X, ixstart, (int)sizeof(double));
  n = i2 - ix;
  for (ixstart = 0; ixstart < n; ixstart++) {
    for (i = 0; i < 1764; i++) {
      b_X->data[i + b_X->size[0] * ixstart] = X->data[i + X->size[0] * (ix +
        ixstart)];
    }
  }

  for (ixstart = 0; ixstart < 2; ixstart++) {
    sz[ixstart] = b_X->size[ixstart];
  }

  emxFree_real_T(&b_X);
  emxInit_real_T(&y, 2);
  ixstart = y->size[0] * y->size[1];
  y->size[0] = 1764;
  y->size[1] = sz[1];
  emxEnsureCapacity((emxArray__common *)y, ixstart, (int)sizeof(double));
  i2 = 1764 * (i2 - ix);
  for (ixstart = 0; ixstart < i2; ixstart++) {
    y->data[ixstart] = fabs(X->data[ixstart % 1764 + X->size[0] * (ix +
      div_nzp_s32_floor(ixstart, 1764))]);
  }

  emxFree_real_T(&X);
  for (ix = 0; ix < 2; ix++) {
    sz[ix] = y->size[ix];
  }

  emxInit_real_T(&b_y, 2);
  ix = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = sz[1];
  emxEnsureCapacity((emxArray__common *)b_y, ix, (int)sizeof(double));
  if (y->size[1] == 0) {
    ix = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, ix, (int)sizeof(double));
    ix = b_y->size[0] * b_y->size[1];
    b_y->size[1] = sz[1];
    emxEnsureCapacity((emxArray__common *)b_y, ix, (int)sizeof(double));
    n = sz[1];
    for (ix = 0; ix < n; ix++) {
      b_y->data[ix] = 0.0;
    }
  } else {
    ix = 0;
    n = -1;
    for (i = 1; i <= y->size[1]; i++) {
      ixstart = ix;
      ix++;
      buflen = y->data[ixstart];
      for (ixstart = 0; ixstart < 1763; ixstart++) {
        ix++;
        buflen += y->data[ix - 1];
      }

      n++;
      b_y->data[n] = buflen;
    }
  }

  emxFree_real_T(&y);
  b_emxInit_real_T(&amp, 1);
  emxInit_real_T(&r0, 2);
  c_rdivide(b_y, 1764.0, r0);
  ix = amp->size[0];
  amp->size[0] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)amp, ix, (int)sizeof(double));
  n = r0->size[1];
  emxFree_real_T(&b_y);
  for (ix = 0; ix < n; ix++) {
    amp->data[ix] = r0->data[r0->size[0] * ix];
  }

  emxFree_real_T(&r0);

  /*  Loop pre-conditions */
  i_size[0] = 0;
  i_size[1] = 0;

  /*  0-silent wait for voice, 1-voice active, 2-silent sustain */
  emxFree_real_T(&amp);

  /*  Deal with ending in non-entry state */
}

/* End of code generation (vadsplitphon.c) */
