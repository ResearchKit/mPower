/*
 * features_bpa.c
 *
 * Code generation for function 'features_bpa'
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
#include "quantile.h"
#include "sqrt.h"
#include "sum.h"
#include "power.h"
#include "repmat.h"
#include "diff.h"
#include "combine_vector_elements.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void features_bpa(const emxArray_real_T *post, double ft[3])
{
  int ixstart;
  emxArray_real_T *t;
  int idx;
  double xlast;
  int n;
  int ix;
  boolean_T exitg3;
  emxArray_boolean_T *x;
  int ii_data[1];
  int ii;
  boolean_T exitg2;
  int istart_data[1];
  boolean_T exitg1;
  unsigned int iend_data[1];
  emxArray_real_T *posttrim;
  int vstride;
  double dT;
  double b_x[3];
  double c_x[3];
  emxArray_real_T *d_x;
  emxArray_real_T *b_t;
  emxArray_real_T *dt;
  emxArray_real_T *b_dt;
  int k;
  int npages;
  emxArray_real_T *b;
  emxArray_real_T *r11;
  emxArray_real_T *fl;
  double alpha;

  /*  Computes basic posture test features. */
  /*  Inputs: */
  /*   post - posture accelerometry vector: post(:,1) - time points, */
  /*          post(:,2:4) - X,Y,Z acceleration data */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  /*  Output feature vector */
  for (ixstart = 0; ixstart < 3; ixstart++) {
    ft[ixstart] = rtNaN;
  }

  /*  Ignore zero-length inputs */
  if (post->size[0] == 0) {
  } else {
    b_emxInit_real_T(&t, 1);

    /*  Calculate relative time */
    idx = post->size[0];
    xlast = post->data[0];
    ixstart = t->size[0];
    t->size[0] = idx;
    emxEnsureCapacity((emxArray__common *)t, ixstart, (int)sizeof(double));
    for (ixstart = 0; ixstart < idx; ixstart++) {
      t->data[ixstart] = post->data[ixstart] - xlast;
    }

    /*  Ignore posture tests which do not contain enough data */
    ixstart = 1;
    n = t->size[0];
    xlast = t->data[0];
    if (t->size[0] > 1) {
      if (rtIsNaN(t->data[0])) {
        ix = 2;
        exitg3 = false;
        while ((!exitg3) && (ix <= n)) {
          ixstart = ix;
          if (!rtIsNaN(t->data[ix - 1])) {
            xlast = t->data[ix - 1];
            exitg3 = true;
          } else {
            ix++;
          }
        }
      }

      if (ixstart < t->size[0]) {
        while (ixstart + 1 <= n) {
          if (t->data[ixstart] > xlast) {
            xlast = t->data[ixstart];
          }

          ixstart++;
        }
      }
    }

    if (xlast < 19.0) {
    } else {
      emxInit_boolean_T(&x, 1);

      /*  Trim away start/end of test */
      ixstart = x->size[0];
      x->size[0] = t->size[0];
      emxEnsureCapacity((emxArray__common *)x, ixstart, (int)sizeof(boolean_T));
      idx = t->size[0];
      for (ixstart = 0; ixstart < idx; ixstart++) {
        x->data[ixstart] = (t->data[ixstart] >= 3.0);
      }

      idx = 0;
      n = 1;
      ii = 1;
      exitg2 = false;
      while ((!exitg2) && (ii <= x->size[0])) {
        if (x->data[ii - 1]) {
          idx = 1;
          ii_data[0] = ii;
          exitg2 = true;
        } else {
          ii++;
        }
      }

      if (idx == 0) {
        n = 0;
      }

      ixstart = 0;
      while (ixstart <= n - 1) {
        istart_data[0] = ii_data[0];
        ixstart = 1;
      }

      ixstart = x->size[0];
      x->size[0] = t->size[0];
      emxEnsureCapacity((emxArray__common *)x, ixstart, (int)sizeof(boolean_T));
      idx = t->size[0];
      for (ixstart = 0; ixstart < idx; ixstart++) {
        x->data[ixstart] = (t->data[ixstart] <= 19.0);
      }

      idx = 0;
      n = 1;
      ii = x->size[0];
      exitg1 = false;
      while ((!exitg1) && (ii > 0)) {
        if (x->data[ii - 1]) {
          idx = 1;
          ii_data[0] = ii;
          exitg1 = true;
        } else {
          ii--;
        }
      }

      emxFree_boolean_T(&x);
      if (idx == 0) {
        n = 0;
      }

      ixstart = 0;
      while (ixstart <= n - 1) {
        iend_data[0] = (unsigned int)ii_data[0];
        ixstart = 1;
      }

      if ((int)iend_data[0] < istart_data[0]) {
      } else {
        if (istart_data[0] > (int)iend_data[0]) {
          ixstart = 1;
          ii = 1;
        } else {
          ixstart = istart_data[0];
          ii = (int)iend_data[0] + 1;
        }

        emxInit_real_T(&posttrim, 2);
        ix = posttrim->size[0] * posttrim->size[1];
        posttrim->size[0] = ii - ixstart;
        posttrim->size[1] = 3;
        emxEnsureCapacity((emxArray__common *)posttrim, ix, (int)sizeof(double));
        for (ix = 0; ix < 3; ix++) {
          idx = ii - ixstart;
          for (vstride = 0; vstride < idx; vstride++) {
            posttrim->data[vstride + posttrim->size[0] * ix] = post->data
              [((ixstart + vstride) + post->size[0] * (1 + ix)) - 1];
          }
        }

        if (istart_data[0] > (int)iend_data[0]) {
          ix = 0;
          vstride = 0;
        } else {
          ix = istart_data[0] - 1;
          vstride = (int)iend_data[0];
        }

        if (0 == ii - ixstart) {
          n = -1;
        } else if (ii - ixstart > 3) {
          n = (ii - ixstart) - 1;
        } else {
          n = 2;
        }

        dT = t->data[((ix + vstride) - ix) - 1] - t->data[ix];

        /*  Orientation */
        combine_vector_elements(posttrim, b_x);
        idx = ii - ixstart;

        /*  Orientation-corrected force signals */
        for (ixstart = 0; ixstart < 3; ixstart++) {
          c_x[ixstart] = b_x[ixstart] / (double)idx;
        }

        emxInit_real_T(&d_x, 2);
        repmat(c_x, n + 1, d_x);
        ixstart = posttrim->size[0] * posttrim->size[1];
        posttrim->size[1] = 3;
        emxEnsureCapacity((emxArray__common *)posttrim, ixstart, (int)sizeof
                          (double));
        idx = posttrim->size[0];
        ii = posttrim->size[1];
        idx *= ii;
        for (ixstart = 0; ixstart < idx; ixstart++) {
          posttrim->data[ixstart] -= d_x->data[ixstart];
        }

        b_emxInit_real_T(&b_t, 1);

        /*  Scaled velocity signals */
        ixstart = b_t->size[0];
        b_t->size[0] = vstride - ix;
        emxEnsureCapacity((emxArray__common *)b_t, ixstart, (int)sizeof(double));
        idx = vstride - ix;
        for (ixstart = 0; ixstart < idx; ixstart++) {
          b_t->data[ixstart] = t->data[ix + ixstart];
        }

        b_emxInit_real_T(&dt, 1);
        b_emxInit_real_T(&b_dt, 1);
        diff(b_t, dt);

        /* dt(end+1) = dt(end); -- causes code gen bug (doesn't update size of dt) */
        idx = dt->size[0] - 1;
        xlast = dt->data[idx];
        ixstart = b_dt->size[0];
        b_dt->size[0] = dt->size[0] + 1;
        emxEnsureCapacity((emxArray__common *)b_dt, ixstart, (int)sizeof(double));
        idx = dt->size[0];
        emxFree_real_T(&b_t);
        for (ixstart = 0; ixstart < idx; ixstart++) {
          b_dt->data[ixstart] = dt->data[ixstart];
        }

        b_dt->data[dt->size[0]] = xlast;
        ixstart = dt->size[0];
        dt->size[0] = b_dt->size[0];
        emxEnsureCapacity((emxArray__common *)dt, ixstart, (int)sizeof(double));
        idx = b_dt->size[0];
        for (ixstart = 0; ixstart < idx; ixstart++) {
          dt->data[ixstart] = b_dt->data[ixstart];
        }

        emxFree_real_T(&b_dt);
        b_repmat(dt, d_x);
        ixstart = d_x->size[0] * d_x->size[1];
        d_x->size[0] = posttrim->size[0];
        d_x->size[1] = 3;
        emxEnsureCapacity((emxArray__common *)d_x, ixstart, (int)sizeof(double));
        idx = posttrim->size[0] * posttrim->size[1];
        for (ixstart = 0; ixstart < idx; ixstart++) {
          d_x->data[ixstart] *= posttrim->data[ixstart];
        }

        if (d_x->size[0] != 1) {
          idx = 0;
        } else {
          idx = 1;
        }

        n = d_x->size[idx];
        if (d_x->size[idx] > 1) {
          vstride = 1;
          k = 1;
          while (k <= idx) {
            vstride *= d_x->size[0];
            k = 2;
          }

          npages = 1;
          k = idx + 2;
          while (k < 3) {
            npages *= 3;
            k = 3;
          }

          ix = 0;
          for (idx = 1; idx <= npages; idx++) {
            ixstart = ix;
            for (ii = 1; ii <= vstride; ii++) {
              ixstart++;
              ix = ixstart;
              xlast = d_x->data[ixstart - 1];
              for (k = 0; k <= n - 2; k++) {
                ix += vstride;
                xlast += d_x->data[ix - 1];
                d_x->data[ix - 1] = xlast;
              }
            }
          }
        }

        emxInit_real_T(&b, 2);
        emxInit_real_T(&r11, 2);

        /*  Average scaled power X,Y,Z */
        power(d_x, b);
        ixstart = r11->size[0] * r11->size[1];
        r11->size[0] = b->size[0];
        r11->size[1] = 3;
        emxEnsureCapacity((emxArray__common *)r11, ixstart, (int)sizeof(double));
        idx = b->size[0] * b->size[1];
        for (ixstart = 0; ixstart < idx; ixstart++) {
          r11->data[ixstart] = 35.0 * b->data[ixstart];
        }

        emxFree_real_T(&b);
        sum(r11, b_x);
        emxFree_real_T(&r11);
        for (ixstart = 0; ixstart < 3; ixstart++) {
          b_x[ixstart] /= dT;
        }

        xlast = b_x[0];
        for (k = 0; k < 2; k++) {
          xlast += b_x[k + 1];
        }

        b_emxInit_real_T(&fl, 1);

        /*  Force vector magnitude signal */
        power(posttrim, d_x);
        b_sum(d_x, t);
        b_sqrt(t);

        /*  Maximum force */
        dT = quantile(t);

        /*  Detrended fluctuation analysis scaling exponent */
        fastdfa(t, &alpha, dt, fl);

        /*  Output posture test feature vector */
        ft[0] = dT / 10.0;
        ft[1] = xlast / 3.0 / 10000.0;
        ft[2] = alpha;
        emxFree_real_T(&d_x);
        emxFree_real_T(&fl);
        emxFree_real_T(&dt);
        emxFree_real_T(&posttrim);
      }
    }

    emxFree_real_T(&t);
  }
}

/* End of code generation (features_bpa.c) */
