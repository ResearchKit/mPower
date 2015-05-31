/*
 * features_bga.c
 *
 * Code generation for function 'features_bga'
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
#include "median.h"
#include "log10.h"
#include "iqr.h"
#include "diff.h"
#include "mean.h"
#include "quantile.h"
#include "sqrt.h"
#include "sum.h"
#include "power.h"
#include "repmat.h"
#include "combine_vector_elements.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void eml_li_find(const emxArray_boolean_T *x, emxArray_int32_T *y)
{
  int n;
  int k;
  int i;
  int j;
  n = x->size[0];
  k = 0;
  for (i = 1; i <= n; i++) {
    if (x->data[i - 1]) {
      k++;
    }
  }

  j = y->size[0];
  y->size[0] = k;
  emxEnsureCapacity((emxArray__common *)y, j, (int)sizeof(int));
  j = 0;
  for (i = 1; i <= n; i++) {
    if (x->data[i - 1]) {
      y->data[j] = i;
      j++;
    }
  }
}

void features_bga(const emxArray_real_T *gait, double ft[7])
{
  int npages;
  emxArray_real_T *t;
  int n;
  double mg;
  int ixstart;
  int ix;
  boolean_T exitg4;
  emxArray_boolean_T *x;
  int idx;
  int ii_data[1];
  int ii_size_idx_0;
  boolean_T exitg3;
  int istart_data[1];
  boolean_T exitg2;
  unsigned int iend_data[1];
  emxArray_real_T *gaittrim;
  int vlen;
  int vstride;
  emxArray_real_T *ttrim;
  double dT;
  double b_x[3];
  double c_x[3];
  emxArray_real_T *d_x;
  emxArray_real_T *dt;
  emxArray_real_T *b_dt;
  emxArray_real_T *b;
  emxArray_real_T *r1;
  double y;
  emxArray_real_T *gaitmag;
  double A;
  emxArray_boolean_T *b_gaitmag;
  emxArray_real_T *tzc;
  emxArray_int32_T *r2;
  double zcr;
  emxArray_real_T *c_gaitmag;
  double zcv;
  boolean_T exitg1;
  emxArray_boolean_T *b_t;

  /*  Computes basic gait test features. */
  /*  Inputs: */
  /*   gait - gait accelerometry vector: gait(:,1) - time points, */
  /*          gait(:,2:4) - X,Y,Z acceleration data */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  /*  Output feature vector */
  for (npages = 0; npages < 7; npages++) {
    ft[npages] = rtNaN;
  }

  /*  Ignore zero-length inputs */
  if (gait->size[0] == 0) {
  } else {
    b_emxInit_real_T(&t, 1);

    /*  Calculate relative time */
    n = gait->size[0];
    mg = gait->data[0];
    npages = t->size[0];
    t->size[0] = n;
    emxEnsureCapacity((emxArray__common *)t, npages, (int)sizeof(double));
    for (npages = 0; npages < n; npages++) {
      t->data[npages] = gait->data[npages] - mg;
    }

    /*  Ignore gait tests which do not contain enough data */
    ixstart = 1;
    n = t->size[0];
    mg = t->data[0];
    if (t->size[0] > 1) {
      if (rtIsNaN(t->data[0])) {
        ix = 2;
        exitg4 = false;
        while ((!exitg4) && (ix <= n)) {
          ixstart = ix;
          if (!rtIsNaN(t->data[ix - 1])) {
            mg = t->data[ix - 1];
            exitg4 = true;
          } else {
            ix++;
          }
        }
      }

      if (ixstart < t->size[0]) {
        while (ixstart + 1 <= n) {
          if (t->data[ixstart] > mg) {
            mg = t->data[ixstart];
          }

          ixstart++;
        }
      }
    }

    if (mg < 19.0) {
    } else {
      emxInit_boolean_T(&x, 1);

      /*  Trim away start/end of test */
      npages = x->size[0];
      x->size[0] = t->size[0];
      emxEnsureCapacity((emxArray__common *)x, npages, (int)sizeof(boolean_T));
      n = t->size[0];
      for (npages = 0; npages < n; npages++) {
        x->data[npages] = (t->data[npages] >= 3.0);
      }

      idx = 0;
      ii_size_idx_0 = 1;
      n = 1;
      exitg3 = false;
      while ((!exitg3) && (n <= x->size[0])) {
        if (x->data[n - 1]) {
          idx = 1;
          ii_data[0] = n;
          exitg3 = true;
        } else {
          n++;
        }
      }

      if (idx == 0) {
        ii_size_idx_0 = 0;
      }

      npages = 0;
      while (npages <= ii_size_idx_0 - 1) {
        istart_data[0] = ii_data[0];
        npages = 1;
      }

      npages = x->size[0];
      x->size[0] = t->size[0];
      emxEnsureCapacity((emxArray__common *)x, npages, (int)sizeof(boolean_T));
      n = t->size[0];
      for (npages = 0; npages < n; npages++) {
        x->data[npages] = (t->data[npages] <= 19.0);
      }

      idx = 0;
      ii_size_idx_0 = 1;
      n = x->size[0];
      exitg2 = false;
      while ((!exitg2) && (n > 0)) {
        if (x->data[n - 1]) {
          idx = 1;
          ii_data[0] = n;
          exitg2 = true;
        } else {
          n--;
        }
      }

      emxFree_boolean_T(&x);
      if (idx == 0) {
        ii_size_idx_0 = 0;
      }

      npages = 0;
      while (npages <= ii_size_idx_0 - 1) {
        iend_data[0] = (unsigned int)ii_data[0];
        npages = 1;
      }

      if (istart_data[0] > (int)iend_data[0]) {
        npages = 1;
        ii_size_idx_0 = 1;
      } else {
        npages = istart_data[0];
        ii_size_idx_0 = (int)iend_data[0] + 1;
      }

      emxInit_real_T(&gaittrim, 2);
      vlen = gaittrim->size[0] * gaittrim->size[1];
      gaittrim->size[0] = ii_size_idx_0 - npages;
      gaittrim->size[1] = 3;
      emxEnsureCapacity((emxArray__common *)gaittrim, vlen, (int)sizeof(double));
      for (vlen = 0; vlen < 3; vlen++) {
        n = ii_size_idx_0 - npages;
        for (vstride = 0; vstride < n; vstride++) {
          gaittrim->data[vstride + gaittrim->size[0] * vlen] = gait->data
            [((npages + vstride) + gait->size[0] * (1 + vlen)) - 1];
        }
      }

      if (istart_data[0] > (int)iend_data[0]) {
        vlen = 0;
        vstride = 0;
      } else {
        vlen = istart_data[0] - 1;
        vstride = (int)iend_data[0];
      }

      b_emxInit_real_T(&ttrim, 1);
      idx = ttrim->size[0];
      ttrim->size[0] = vstride - vlen;
      emxEnsureCapacity((emxArray__common *)ttrim, idx, (int)sizeof(double));
      n = vstride - vlen;
      for (idx = 0; idx < n; idx++) {
        ttrim->data[idx] = t->data[vlen + idx];
      }

      if (0 == ii_size_idx_0 - npages) {
        n = -1;
      } else if (ii_size_idx_0 - npages > 3) {
        n = (ii_size_idx_0 - npages) - 1;
      } else {
        n = 2;
      }

      dT = t->data[((vlen + vstride) - vlen) - 1] - t->data[vlen];

      /*  Orientation */
      combine_vector_elements(gaittrim, b_x);
      idx = ii_size_idx_0 - npages;

      /*  Orientation-corrected force signals */
      for (npages = 0; npages < 3; npages++) {
        c_x[npages] = b_x[npages] / (double)idx;
      }

      emxInit_real_T(&d_x, 2);
      repmat(c_x, n + 1, d_x);
      npages = gaittrim->size[0] * gaittrim->size[1];
      gaittrim->size[1] = 3;
      emxEnsureCapacity((emxArray__common *)gaittrim, npages, (int)sizeof(double));
      idx = gaittrim->size[0];
      n = gaittrim->size[1];
      n *= idx;
      for (npages = 0; npages < n; npages++) {
        gaittrim->data[npages] -= d_x->data[npages];
      }

      b_emxInit_real_T(&dt, 1);
      b_emxInit_real_T(&b_dt, 1);

      /*  Scaled velocity signals */
      diff(ttrim, dt);

      /* dt(end+1) = dt(end); -- causes code gen bug (doesn't update size of dt) */
      idx = dt->size[0] - 1;
      mg = dt->data[idx];
      npages = b_dt->size[0];
      b_dt->size[0] = dt->size[0] + 1;
      emxEnsureCapacity((emxArray__common *)b_dt, npages, (int)sizeof(double));
      n = dt->size[0];
      for (npages = 0; npages < n; npages++) {
        b_dt->data[npages] = dt->data[npages];
      }

      b_dt->data[dt->size[0]] = mg;
      npages = dt->size[0];
      dt->size[0] = b_dt->size[0];
      emxEnsureCapacity((emxArray__common *)dt, npages, (int)sizeof(double));
      n = b_dt->size[0];
      for (npages = 0; npages < n; npages++) {
        dt->data[npages] = b_dt->data[npages];
      }

      emxFree_real_T(&b_dt);
      b_repmat(dt, d_x);
      npages = d_x->size[0] * d_x->size[1];
      d_x->size[0] = gaittrim->size[0];
      d_x->size[1] = 3;
      emxEnsureCapacity((emxArray__common *)d_x, npages, (int)sizeof(double));
      n = gaittrim->size[0] * gaittrim->size[1];
      for (npages = 0; npages < n; npages++) {
        d_x->data[npages] *= gaittrim->data[npages];
      }

      if (d_x->size[0] != 1) {
        idx = 0;
      } else {
        idx = 1;
      }

      vlen = d_x->size[idx];
      if (d_x->size[idx] > 1) {
        vstride = 1;
        n = 1;
        while (n <= idx) {
          vstride *= d_x->size[0];
          n = 2;
        }

        npages = 1;
        n = idx + 2;
        while (n < 3) {
          npages *= 3;
          n = 3;
        }

        ix = 0;
        for (idx = 1; idx <= npages; idx++) {
          ixstart = ix;
          for (ii_size_idx_0 = 1; ii_size_idx_0 <= vstride; ii_size_idx_0++) {
            ixstart++;
            ix = ixstart;
            mg = d_x->data[ixstart - 1];
            for (n = 0; n <= vlen - 2; n++) {
              ix += vstride;
              mg += d_x->data[ix - 1];
              d_x->data[ix - 1] = mg;
            }
          }
        }
      }

      emxInit_real_T(&b, 2);
      emxInit_real_T(&r1, 2);

      /*  Average scaled power X,Y,Z */
      power(d_x, b);
      npages = r1->size[0] * r1->size[1];
      r1->size[0] = b->size[0];
      r1->size[1] = 3;
      emxEnsureCapacity((emxArray__common *)r1, npages, (int)sizeof(double));
      n = b->size[0] * b->size[1];
      for (npages = 0; npages < n; npages++) {
        r1->data[npages] = 35.0 * b->data[npages];
      }

      emxFree_real_T(&b);
      sum(r1, b_x);
      emxFree_real_T(&r1);
      for (npages = 0; npages < 3; npages++) {
        b_x[npages] /= dT;
      }

      y = b_x[0];
      for (n = 0; n < 2; n++) {
        y += b_x[n + 1];
      }

      b_emxInit_real_T(&gaitmag, 1);

      /*  Force vector magnitude signal */
      power(gaittrim, d_x);
      b_sum(d_x, gaitmag);
      b_sqrt(gaitmag);

      /*  Maximum force */
      A = quantile(gaitmag);

      /*  Zero crossing events */
      mg = mean(gaitmag);
      emxFree_real_T(&d_x);
      emxFree_real_T(&gaittrim);
      if (1.0 > (double)gaitmag->size[0] - 1.0) {
        n = 0;
      } else {
        n = (int)((double)gaitmag->size[0] - 1.0);
      }

      if (2 > gaitmag->size[0]) {
        npages = 0;
      } else {
        npages = 1;
      }

      emxInit_boolean_T(&b_gaitmag, 1);
      ii_size_idx_0 = b_gaitmag->size[0];
      b_gaitmag->size[0] = n;
      emxEnsureCapacity((emxArray__common *)b_gaitmag, ii_size_idx_0, (int)
                        sizeof(boolean_T));
      for (ii_size_idx_0 = 0; ii_size_idx_0 < n; ii_size_idx_0++) {
        b_gaitmag->data[ii_size_idx_0] = ((gaitmag->data[ii_size_idx_0] < mg) &&
          (gaitmag->data[npages + ii_size_idx_0] >= mg));
      }

      b_emxInit_real_T(&tzc, 1);
      emxInit_int32_T(&r2, 1);
      eml_li_find(b_gaitmag, r2);
      npages = tzc->size[0];
      tzc->size[0] = r2->size[0];
      emxEnsureCapacity((emxArray__common *)tzc, npages, (int)sizeof(double));
      n = r2->size[0];
      emxFree_boolean_T(&b_gaitmag);
      for (npages = 0; npages < n; npages++) {
        tzc->data[npages] = ttrim->data[r2->data[npages] - 1];
      }

      diff(tzc, dt);
      if (dt->size[0] == 0) {
        zcr = rtNaN;
      } else {
        zcr = vectormedian(dt);
      }

      b_emxInit_real_T(&c_gaitmag, 1);
      zcv = iqr(dt);

      /*  Non-uniform frequency features */
      npages = c_gaitmag->size[0];
      c_gaitmag->size[0] = gaitmag->size[0];
      emxEnsureCapacity((emxArray__common *)c_gaitmag, npages, (int)sizeof
                        (double));
      n = gaitmag->size[0];
      for (npages = 0; npages < n; npages++) {
        c_gaitmag->data[npages] = gaitmag->data[npages] - mg;
      }

      b_lomb(ttrim, c_gaitmag, t, gaitmag, tzc);

      /*  Peak frequency */
      ixstart = 1;
      n = gaitmag->size[0];
      mg = gaitmag->data[0];
      idx = 0;
      emxFree_real_T(&c_gaitmag);
      emxFree_real_T(&ttrim);
      if (gaitmag->size[0] > 1) {
        if (rtIsNaN(gaitmag->data[0])) {
          ix = 1;
          exitg1 = false;
          while ((!exitg1) && (ix + 1 <= n)) {
            ixstart = ix + 1;
            if (!rtIsNaN(gaitmag->data[ix])) {
              mg = gaitmag->data[ix];
              idx = ix;
              exitg1 = true;
            } else {
              ix++;
            }
          }
        }

        if (ixstart < gaitmag->size[0]) {
          while (ixstart + 1 <= n) {
            if (gaitmag->data[ixstart] > mg) {
              mg = gaitmag->data[ixstart];
              idx = ixstart;
            }

            ixstart++;
          }
        }
      }

      emxInit_boolean_T(&b_t, 1);

      /*  SNR of peak frequency */
      dT = t->data[idx];
      npages = b_t->size[0];
      b_t->size[0] = t->size[0];
      emxEnsureCapacity((emxArray__common *)b_t, npages, (int)sizeof(boolean_T));
      n = t->size[0];
      for (npages = 0; npages < n; npages++) {
        b_t->data[npages] = (t->data[npages] <= dT);
      }

      eml_li_find(b_t, r2);
      npages = dt->size[0];
      dt->size[0] = r2->size[0];
      emxEnsureCapacity((emxArray__common *)dt, npages, (int)sizeof(double));
      n = r2->size[0];
      emxFree_boolean_T(&b_t);
      for (npages = 0; npages < n; npages++) {
        dt->data[npages] = gaitmag->data[r2->data[npages] - 1];
      }

      emxFree_int32_T(&r2);
      emxFree_real_T(&gaitmag);
      b_log10(dt);
      if (dt->size[0] == 0) {
        dT = rtNaN;
      } else {
        dT = vectormedian(dt);
      }

      emxFree_real_T(&dt);

      /*  Output gait test feature vector */
      ft[0] = A / 10.0;
      ft[1] = y / 3.0 / 10000.0;
      ft[2] = zcr;
      ft[3] = zcv;
      ft[4] = t->data[idx];
      ft[5] = log10(mg) - dT;
      ft[6] = -log10(tzc->data[idx]);
      emxFree_real_T(&tzc);
    }

    emxFree_real_T(&t);
  }
}

/* End of code generation (features_bga.c) */
