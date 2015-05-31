/*
 * lomb.c
 *
 * Code generation for function 'lomb'
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
#include "power.h"
#include "rdivide.h"
#include "sum.h"
#include "repmat.h"
#include "atan2.h"
#include "var.h"
#include "mean.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_lomb(const emxArray_real_T *t, const emxArray_real_T *h, emxArray_real_T *
            f, emxArray_real_T *P, emxArray_real_T *prob)
{
  int ixstart;
  int n;
  double T;
  int ix;
  boolean_T exitg2;
  double ndbl;
  boolean_T exitg1;
  double mu;
  double s2;
  double y;
  double b_y;
  double apnd;
  double cdiff;
  double absa;
  double absb;
  emxArray_real_T *c_y;
  int i8;
  int k;
  int ar;
  emxArray_real_T *w;
  emxArray_real_T *d_y;
  int i9;
  emxArray_real_T *e_y;
  emxArray_real_T *r3;
  emxArray_real_T *r4;
  emxArray_real_T *r5;
  emxArray_real_T *r6;
  emxArray_real_T *tau;
  emxArray_real_T *b_w;
  emxArray_real_T *cterm;
  emxArray_real_T *c_w;
  emxArray_real_T *sterm;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int m;
  int ic;
  int ib;
  int ia;
  emxArray_real_T *r7;
  emxArray_boolean_T *inds;
  emxArray_int32_T *r8;
  emxArray_int32_T *r9;
  emxArray_real_T *r10;

  /*  LOMB(T,H,OFAC,HIFAC) computes the Lomb normalized periodogram (spectral */
  /*  power as a function of frequency) of a sequence of N data points H, */
  /*  sampled at times T, which are not necessarily evenly spaced. T and H must */
  /*  be vectors of equal size. The routine will calculate the spectral power */
  /*  for an increasing sequence of frequencies (in reciprocal units of the */
  /*  time array T) up to HIFAC times the average Nyquist frequency, with an */
  /*  oversampling factor of OFAC (typically >= 4). */
  /*   */
  /*  The returned values are arrays of frequencies considered (f), the */
  /*  associated spectral power (P) and estimated significance of the power */
  /*  values (prob).  Note: the significance returned is the false alarm */
  /*  probability of the null hypothesis, i.e. that the data is composed of */
  /*  independent gaussian random variables.  Low probability values indicate a */
  /*  high degree of significance in the associated periodic signal. */
  /*   */
  /*  Although this implementation is based on that described in Press, */
  /*  Teukolsky, et al. Numerical Recipes  In C, section 13.8, rather than using */
  /*  trigonometric rercurrences, this takes advantage of MATALB's array */
  /*  operators to calculate the exact spectral power as defined in equation */
  /*  13.8.4 on page 577.  This may cause memory issues for large data sets and */
  /*  frequency ranges. */
  /*   */
  /*  Example     */
  /*     [f,P,prob] = lomb(t,h,4,1);    */
  /*     plot(f,P) */
  /*     [Pmax,jmax] = max(P) */
  /*     disp(['Most significant period is ',num2str(1/f(jmax)),... */
  /*          ' with FAP of ',num2str(prob(jmax))]) */
  /*   */
  /*  Written by Dmitry Savransky 21 May 2008 */
  /* sample length and time span */
  ixstart = 1;
  n = t->size[0];
  T = t->data[0];
  if (t->size[0] > 1) {
    if (rtIsNaN(t->data[0])) {
      ix = 2;
      exitg2 = false;
      while ((!exitg2) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(t->data[ix - 1])) {
          T = t->data[ix - 1];
          exitg2 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < t->size[0]) {
      while (ixstart + 1 <= n) {
        if (t->data[ixstart] > T) {
          T = t->data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = t->size[0];
  ndbl = t->data[0];
  if (t->size[0] > 1) {
    if (rtIsNaN(t->data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(t->data[ix - 1])) {
          ndbl = t->data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < t->size[0]) {
      while (ixstart + 1 <= n) {
        if (t->data[ixstart] < ndbl) {
          ndbl = t->data[ixstart];
        }

        ixstart++;
      }
    }
  }

  T -= ndbl;

  /* mean and variance  */
  mu = mean(h);
  s2 = var(h);

  /* calculate sampling frequencies */
  y = 1.0 / (T * 4.0);
  b_y = 1.0 / (T * 4.0);
  T = (double)h->size[0] / (2.0 * T);
  if (rtIsNaN(y) || rtIsNaN(b_y) || rtIsNaN(T)) {
    n = 0;
    y = rtNaN;
    apnd = T;
  } else if ((b_y == 0.0) || ((y < T) && (b_y < 0.0)) || ((T < y) && (b_y > 0.0)))
  {
    n = -1;
    apnd = T;
  } else if (rtIsInf(y) || rtIsInf(T)) {
    n = 0;
    y = rtNaN;
    apnd = T;
  } else if (rtIsInf(b_y)) {
    n = 0;
    apnd = T;
  } else {
    ndbl = floor((T - y) / b_y + 0.5);
    apnd = y + ndbl * b_y;
    if (b_y > 0.0) {
      cdiff = apnd - T;
    } else {
      cdiff = T - apnd;
    }

    absa = fabs(y);
    absb = fabs(T);
    if (fabs(cdiff) < 4.4408920985006262E-16 * fmax(absa, absb)) {
      ndbl++;
      apnd = T;
    } else if (cdiff > 0.0) {
      apnd = y + (ndbl - 1.0) * b_y;
    } else {
      ndbl++;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl - 1;
    } else {
      n = -1;
    }
  }

  emxInit_real_T(&c_y, 2);
  i8 = c_y->size[0] * c_y->size[1];
  c_y->size[0] = 1;
  c_y->size[1] = n + 1;
  emxEnsureCapacity((emxArray__common *)c_y, i8, (int)sizeof(double));
  if (n + 1 > 0) {
    c_y->data[0] = y;
    if (n + 1 > 1) {
      c_y->data[n] = apnd;
      ixstart = (n + (n < 0)) >> 1;
      for (k = 1; k < ixstart; k++) {
        ndbl = (double)k * b_y;
        c_y->data[k] = y + ndbl;
        c_y->data[n - k] = apnd - ndbl;
      }

      if (ixstart << 1 == n) {
        c_y->data[ixstart] = (y + apnd) / 2.0;
      } else {
        ndbl = (double)ixstart * b_y;
        c_y->data[ixstart] = y + ndbl;
        c_y->data[ixstart + 1] = apnd - ndbl;
      }
    }
  }

  i8 = f->size[0];
  f->size[0] = c_y->size[1];
  emxEnsureCapacity((emxArray__common *)f, i8, (int)sizeof(double));
  ar = c_y->size[1];
  for (i8 = 0; i8 < ar; i8++) {
    f->data[i8] = c_y->data[c_y->size[0] * i8];
  }

  emxFree_real_T(&c_y);
  b_emxInit_real_T(&w, 1);

  /* angular frequencies and constant offsets */
  i8 = w->size[0];
  w->size[0] = f->size[0];
  emxEnsureCapacity((emxArray__common *)w, i8, (int)sizeof(double));
  ar = f->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    w->data[i8] = 6.2831853071795862 * f->data[i8];
  }

  emxInit_real_T(&d_y, 2);
  i8 = d_y->size[0] * d_y->size[1];
  d_y->size[0] = w->size[0];
  d_y->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
  ar = w->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    ixstart = t->size[0];
    for (i9 = 0; i9 < ixstart; i9++) {
      d_y->data[i8 + d_y->size[0] * i9] = 2.0 * w->data[i8] * t->data[i9];
    }
  }

  i8 = d_y->size[0] * d_y->size[1];
  for (k = 0; k < i8; k++) {
    d_y->data[k] = sin(d_y->data[k]);
  }

  emxInit_real_T(&e_y, 2);
  i8 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = w->size[0];
  e_y->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)e_y, i8, (int)sizeof(double));
  ar = w->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    ixstart = t->size[0];
    for (i9 = 0; i9 < ixstart; i9++) {
      e_y->data[i8 + e_y->size[0] * i9] = 2.0 * w->data[i8] * t->data[i9];
    }
  }

  i8 = e_y->size[0] * e_y->size[1];
  for (k = 0; k < i8; k++) {
    e_y->data[k] = cos(e_y->data[k]);
  }

  b_emxInit_real_T(&r3, 1);
  b_emxInit_real_T(&r4, 1);
  b_emxInit_real_T(&r5, 1);
  b_emxInit_real_T(&r6, 1);
  c_sum(d_y, r3);
  c_sum(e_y, r4);
  b_atan2(r3, r4, r5);
  i8 = r6->size[0];
  r6->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)r6, i8, (int)sizeof(double));
  ar = w->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    r6->data[i8] = 2.0 * w->data[i8];
  }

  b_emxInit_real_T(&tau, 1);
  b_emxInit_real_T(&b_w, 1);
  d_rdivide(r5, r6, tau);

  /* spectral power */
  i8 = b_w->size[0];
  b_w->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)b_w, i8, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&r6);
  for (i8 = 0; i8 < ar; i8++) {
    b_w->data[i8] = w->data[i8] * tau->data[i8];
  }

  emxInit_real_T(&cterm, 2);
  c_repmat(b_w, t->size[0], e_y);
  i8 = cterm->size[0] * cterm->size[1];
  cterm->size[0] = w->size[0];
  cterm->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)cterm, i8, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&b_w);
  for (i8 = 0; i8 < ar; i8++) {
    ixstart = t->size[0];
    for (i9 = 0; i9 < ixstart; i9++) {
      T = w->data[i8] * t->data[i9];
      cterm->data[i8 + cterm->size[0] * i9] = T - e_y->data[i8 + e_y->size[0] *
        i9];
    }
  }

  i8 = cterm->size[0] * cterm->size[1];
  for (k = 0; k < i8; k++) {
    cterm->data[k] = cos(cterm->data[k]);
  }

  b_emxInit_real_T(&c_w, 1);
  i8 = c_w->size[0];
  c_w->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)c_w, i8, (int)sizeof(double));
  ar = w->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    c_w->data[i8] = w->data[i8] * tau->data[i8];
  }

  emxInit_real_T(&sterm, 2);
  c_repmat(c_w, t->size[0], e_y);
  i8 = sterm->size[0] * sterm->size[1];
  sterm->size[0] = w->size[0];
  sterm->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)sterm, i8, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&c_w);
  for (i8 = 0; i8 < ar; i8++) {
    ixstart = t->size[0];
    for (i9 = 0; i9 < ixstart; i9++) {
      T = w->data[i8] * t->data[i9];
      sterm->data[i8 + sterm->size[0] * i9] = T - e_y->data[i8 + e_y->size[0] *
        i9];
    }
  }

  i8 = sterm->size[0] * sterm->size[1];
  for (k = 0; k < i8; k++) {
    sterm->data[k] = sin(sterm->data[k]);
  }

  i8 = tau->size[0];
  tau->size[0] = h->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i8, (int)sizeof(double));
  ar = h->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    tau->data[i8] = h->data[i8] - mu;
  }

  ixstart = tau->size[0];
  ix = tau->size[0];
  i8 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)e_y, i8, (int)sizeof(double));
  i8 = e_y->size[0] * e_y->size[1];
  e_y->size[1] = ix;
  emxEnsureCapacity((emxArray__common *)e_y, i8, (int)sizeof(double));
  ar = ixstart * ix;
  for (i8 = 0; i8 < ar; i8++) {
    e_y->data[i8] = 0.0;
  }

  for (ixstart = 0; ixstart + 1 <= tau->size[0]; ixstart++) {
    e_y->data[ixstart + e_y->size[0] * ixstart] = tau->data[ixstart];
  }

  if ((cterm->size[1] == 1) || (e_y->size[0] == 1)) {
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = cterm->size[0];
    d_y->size[1] = e_y->size[1];
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    ar = cterm->size[0];
    for (i8 = 0; i8 < ar; i8++) {
      ixstart = e_y->size[1];
      for (i9 = 0; i9 < ixstart; i9++) {
        d_y->data[i8 + d_y->size[0] * i9] = 0.0;
        ix = cterm->size[1];
        for (n = 0; n < ix; n++) {
          d_y->data[i8 + d_y->size[0] * i9] += cterm->data[i8 + cterm->size[0] *
            n] * e_y->data[n + e_y->size[0] * i9];
        }
      }
    }
  } else {
    k = cterm->size[1];
    unnamed_idx_0 = (unsigned int)cterm->size[0];
    unnamed_idx_1 = (unsigned int)e_y->size[1];
    m = cterm->size[0];
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    ar = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i8 = 0; i8 < ar; i8++) {
      d_y->data[i8] = 0.0;
    }

    if ((cterm->size[0] == 0) || (e_y->size[1] == 0)) {
    } else {
      ixstart = cterm->size[0] * (e_y->size[1] - 1);
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        i8 = ix + m;
        for (ic = ix; ic + 1 <= i8; ic++) {
          d_y->data[ic] = 0.0;
        }

        ix += m;
      }

      n = 0;
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        ar = 0;
        i8 = n + k;
        for (ib = n; ib + 1 <= i8; ib++) {
          if (e_y->data[ib] != 0.0) {
            ia = ar;
            i9 = ix + m;
            for (ic = ix; ic + 1 <= i9; ic++) {
              ia++;
              d_y->data[ic] += e_y->data[ib] * cterm->data[ia - 1];
            }
          }

          ar += m;
        }

        n += k;
        ix += m;
      }
    }
  }

  c_sum(d_y, r3);
  b_power(r3, r4);
  c_power(cterm, e_y);
  c_sum(e_y, r3);
  d_rdivide(r4, r3, r5);
  i8 = tau->size[0];
  tau->size[0] = h->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i8, (int)sizeof(double));
  ar = h->size[0];
  emxFree_real_T(&cterm);
  for (i8 = 0; i8 < ar; i8++) {
    tau->data[i8] = h->data[i8] - mu;
  }

  ixstart = tau->size[0];
  ix = tau->size[0];
  i8 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)e_y, i8, (int)sizeof(double));
  i8 = e_y->size[0] * e_y->size[1];
  e_y->size[1] = ix;
  emxEnsureCapacity((emxArray__common *)e_y, i8, (int)sizeof(double));
  ar = ixstart * ix;
  for (i8 = 0; i8 < ar; i8++) {
    e_y->data[i8] = 0.0;
  }

  for (ixstart = 0; ixstart + 1 <= tau->size[0]; ixstart++) {
    e_y->data[ixstart + e_y->size[0] * ixstart] = tau->data[ixstart];
  }

  if ((sterm->size[1] == 1) || (e_y->size[0] == 1)) {
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = sterm->size[0];
    d_y->size[1] = e_y->size[1];
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    ar = sterm->size[0];
    for (i8 = 0; i8 < ar; i8++) {
      ixstart = e_y->size[1];
      for (i9 = 0; i9 < ixstart; i9++) {
        d_y->data[i8 + d_y->size[0] * i9] = 0.0;
        ix = sterm->size[1];
        for (n = 0; n < ix; n++) {
          d_y->data[i8 + d_y->size[0] * i9] += sterm->data[i8 + sterm->size[0] *
            n] * e_y->data[n + e_y->size[0] * i9];
        }
      }
    }
  } else {
    k = sterm->size[1];
    unnamed_idx_0 = (unsigned int)sterm->size[0];
    unnamed_idx_1 = (unsigned int)e_y->size[1];
    m = sterm->size[0];
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    i8 = d_y->size[0] * d_y->size[1];
    d_y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)d_y, i8, (int)sizeof(double));
    ar = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i8 = 0; i8 < ar; i8++) {
      d_y->data[i8] = 0.0;
    }

    if ((sterm->size[0] == 0) || (e_y->size[1] == 0)) {
    } else {
      ixstart = sterm->size[0] * (e_y->size[1] - 1);
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        i8 = ix + m;
        for (ic = ix; ic + 1 <= i8; ic++) {
          d_y->data[ic] = 0.0;
        }

        ix += m;
      }

      n = 0;
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        ar = 0;
        i8 = n + k;
        for (ib = n; ib + 1 <= i8; ib++) {
          if (e_y->data[ib] != 0.0) {
            ia = ar;
            i9 = ix + m;
            for (ic = ix; ic + 1 <= i9; ic++) {
              ia++;
              d_y->data[ic] += e_y->data[ib] * sterm->data[ia - 1];
            }
          }

          ar += m;
        }

        n += k;
        ix += m;
      }
    }
  }

  b_emxInit_real_T(&r7, 1);
  c_sum(d_y, r3);
  b_power(r3, r4);
  c_power(sterm, e_y);
  c_sum(e_y, r3);
  d_rdivide(r4, r3, tau);
  i8 = r7->size[0];
  r7->size[0] = r5->size[0];
  emxEnsureCapacity((emxArray__common *)r7, i8, (int)sizeof(double));
  ar = r5->size[0];
  emxFree_real_T(&r4);
  emxFree_real_T(&e_y);
  emxFree_real_T(&d_y);
  emxFree_real_T(&sterm);
  for (i8 = 0; i8 < ar; i8++) {
    r7->data[i8] = r5->data[i8] + tau->data[i8];
  }

  emxFree_real_T(&r5);
  rdivide(r7, 2.0 * s2, P);

  /* estimate of the number of independent frequencies */
  T = 2.0 * (double)f->size[0] / 4.0;

  /* statistical significane of power */
  i8 = tau->size[0];
  tau->size[0] = P->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i8, (int)sizeof(double));
  ar = P->size[0];
  emxFree_real_T(&r7);
  for (i8 = 0; i8 < ar; i8++) {
    tau->data[i8] = -P->data[i8];
  }

  i8 = prob->size[0];
  prob->size[0] = tau->size[0];
  emxEnsureCapacity((emxArray__common *)prob, i8, (int)sizeof(double));
  ar = tau->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    prob->data[i8] = tau->data[i8];
  }

  for (k = 0; k < tau->size[0]; k++) {
    prob->data[k] = exp(prob->data[k]);
  }

  i8 = prob->size[0];
  emxEnsureCapacity((emxArray__common *)prob, i8, (int)sizeof(double));
  ar = prob->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    prob->data[i8] *= T;
  }

  emxInit_boolean_T(&inds, 1);
  i8 = inds->size[0];
  inds->size[0] = prob->size[0];
  emxEnsureCapacity((emxArray__common *)inds, i8, (int)sizeof(boolean_T));
  ar = prob->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    inds->data[i8] = (prob->data[i8] > 0.01);
  }

  emxInit_int32_T(&r8, 1);
  emxInit_int32_T(&r9, 1);
  eml_li_find(inds, r8);
  eml_li_find(inds, r9);
  i8 = tau->size[0];
  tau->size[0] = r9->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i8, (int)sizeof(double));
  ar = r9->size[0];
  emxFree_boolean_T(&inds);
  for (i8 = 0; i8 < ar; i8++) {
    tau->data[i8] = -P->data[r9->data[i8] - 1];
  }

  emxFree_int32_T(&r9);
  i8 = w->size[0];
  w->size[0] = tau->size[0];
  emxEnsureCapacity((emxArray__common *)w, i8, (int)sizeof(double));
  ar = tau->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    w->data[i8] = tau->data[i8];
  }

  for (k = 0; k < tau->size[0]; k++) {
    w->data[k] = exp(w->data[k]);
  }

  emxFree_real_T(&tau);
  b_emxInit_real_T(&r10, 1);
  i8 = r10->size[0];
  r10->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)r10, i8, (int)sizeof(double));
  ar = w->size[0];
  for (i8 = 0; i8 < ar; i8++) {
    r10->data[i8] = 1.0 - w->data[i8];
  }

  emxFree_real_T(&w);
  d_power(r10, T, r3);
  ar = r3->size[0];
  emxFree_real_T(&r10);
  for (i8 = 0; i8 < ar; i8++) {
    prob->data[r8->data[i8] - 1] = 1.0 - r3->data[i8];
  }

  emxFree_real_T(&r3);
  emxFree_int32_T(&r8);
}

void lomb(const emxArray_real_T *t, const emxArray_real_T *h, double ofac,
          double hifac, emxArray_real_T *f, emxArray_real_T *P, emxArray_real_T *
          prob)
{
  int ixstart;
  int n;
  double T;
  int ix;
  boolean_T exitg2;
  double ndbl;
  boolean_T exitg1;
  double mu;
  double s2;
  double y;
  double b_y;
  double apnd;
  double cdiff;
  double absa;
  double absb;
  emxArray_real_T *c_y;
  int i25;
  int k;
  int ar;
  emxArray_real_T *w;
  emxArray_real_T *d_y;
  int i26;
  emxArray_real_T *e_y;
  emxArray_real_T *r27;
  emxArray_real_T *r28;
  emxArray_real_T *r29;
  emxArray_real_T *r30;
  emxArray_real_T *tau;
  emxArray_real_T *b_w;
  emxArray_real_T *cterm;
  emxArray_real_T *c_w;
  emxArray_real_T *sterm;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int m;
  int ic;
  int ib;
  int ia;
  emxArray_real_T *r31;
  emxArray_boolean_T *inds;
  emxArray_int32_T *r32;
  emxArray_int32_T *r33;
  emxArray_real_T *r34;

  /*  LOMB(T,H,OFAC,HIFAC) computes the Lomb normalized periodogram (spectral */
  /*  power as a function of frequency) of a sequence of N data points H, */
  /*  sampled at times T, which are not necessarily evenly spaced. T and H must */
  /*  be vectors of equal size. The routine will calculate the spectral power */
  /*  for an increasing sequence of frequencies (in reciprocal units of the */
  /*  time array T) up to HIFAC times the average Nyquist frequency, with an */
  /*  oversampling factor of OFAC (typically >= 4). */
  /*   */
  /*  The returned values are arrays of frequencies considered (f), the */
  /*  associated spectral power (P) and estimated significance of the power */
  /*  values (prob).  Note: the significance returned is the false alarm */
  /*  probability of the null hypothesis, i.e. that the data is composed of */
  /*  independent gaussian random variables.  Low probability values indicate a */
  /*  high degree of significance in the associated periodic signal. */
  /*   */
  /*  Although this implementation is based on that described in Press, */
  /*  Teukolsky, et al. Numerical Recipes  In C, section 13.8, rather than using */
  /*  trigonometric rercurrences, this takes advantage of MATALB's array */
  /*  operators to calculate the exact spectral power as defined in equation */
  /*  13.8.4 on page 577.  This may cause memory issues for large data sets and */
  /*  frequency ranges. */
  /*   */
  /*  Example     */
  /*     [f,P,prob] = lomb(t,h,4,1);    */
  /*     plot(f,P) */
  /*     [Pmax,jmax] = max(P) */
  /*     disp(['Most significant period is ',num2str(1/f(jmax)),... */
  /*          ' with FAP of ',num2str(prob(jmax))]) */
  /*   */
  /*  Written by Dmitry Savransky 21 May 2008 */
  /* sample length and time span */
  ixstart = 1;
  n = t->size[0];
  T = t->data[0];
  if (t->size[0] > 1) {
    if (rtIsNaN(t->data[0])) {
      ix = 2;
      exitg2 = false;
      while ((!exitg2) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(t->data[ix - 1])) {
          T = t->data[ix - 1];
          exitg2 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < t->size[0]) {
      while (ixstart + 1 <= n) {
        if (t->data[ixstart] > T) {
          T = t->data[ixstart];
        }

        ixstart++;
      }
    }
  }

  ixstart = 1;
  n = t->size[0];
  ndbl = t->data[0];
  if (t->size[0] > 1) {
    if (rtIsNaN(t->data[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix <= n)) {
        ixstart = ix;
        if (!rtIsNaN(t->data[ix - 1])) {
          ndbl = t->data[ix - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < t->size[0]) {
      while (ixstart + 1 <= n) {
        if (t->data[ixstart] < ndbl) {
          ndbl = t->data[ixstart];
        }

        ixstart++;
      }
    }
  }

  T -= ndbl;

  /* mean and variance  */
  mu = mean(h);
  s2 = var(h);

  /* calculate sampling frequencies */
  y = 1.0 / (T * ofac);
  b_y = 1.0 / (T * ofac);
  T = hifac * (double)h->size[0] / (2.0 * T);
  if (rtIsNaN(y) || rtIsNaN(b_y) || rtIsNaN(T)) {
    n = 0;
    y = rtNaN;
    apnd = T;
  } else if ((b_y == 0.0) || ((y < T) && (b_y < 0.0)) || ((T < y) && (b_y > 0.0)))
  {
    n = -1;
    apnd = T;
  } else if (rtIsInf(y) || rtIsInf(T)) {
    n = 0;
    y = rtNaN;
    apnd = T;
  } else if (rtIsInf(b_y)) {
    n = 0;
    apnd = T;
  } else {
    ndbl = floor((T - y) / b_y + 0.5);
    apnd = y + ndbl * b_y;
    if (b_y > 0.0) {
      cdiff = apnd - T;
    } else {
      cdiff = T - apnd;
    }

    absa = fabs(y);
    absb = fabs(T);
    if (fabs(cdiff) < 4.4408920985006262E-16 * fmax(absa, absb)) {
      ndbl++;
      apnd = T;
    } else if (cdiff > 0.0) {
      apnd = y + (ndbl - 1.0) * b_y;
    } else {
      ndbl++;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl - 1;
    } else {
      n = -1;
    }
  }

  emxInit_real_T(&c_y, 2);
  i25 = c_y->size[0] * c_y->size[1];
  c_y->size[0] = 1;
  c_y->size[1] = n + 1;
  emxEnsureCapacity((emxArray__common *)c_y, i25, (int)sizeof(double));
  if (n + 1 > 0) {
    c_y->data[0] = y;
    if (n + 1 > 1) {
      c_y->data[n] = apnd;
      ixstart = (n + (n < 0)) >> 1;
      for (k = 1; k < ixstart; k++) {
        ndbl = (double)k * b_y;
        c_y->data[k] = y + ndbl;
        c_y->data[n - k] = apnd - ndbl;
      }

      if (ixstart << 1 == n) {
        c_y->data[ixstart] = (y + apnd) / 2.0;
      } else {
        ndbl = (double)ixstart * b_y;
        c_y->data[ixstart] = y + ndbl;
        c_y->data[ixstart + 1] = apnd - ndbl;
      }
    }
  }

  i25 = f->size[0];
  f->size[0] = c_y->size[1];
  emxEnsureCapacity((emxArray__common *)f, i25, (int)sizeof(double));
  ar = c_y->size[1];
  for (i25 = 0; i25 < ar; i25++) {
    f->data[i25] = c_y->data[c_y->size[0] * i25];
  }

  emxFree_real_T(&c_y);
  b_emxInit_real_T(&w, 1);

  /* angular frequencies and constant offsets */
  i25 = w->size[0];
  w->size[0] = f->size[0];
  emxEnsureCapacity((emxArray__common *)w, i25, (int)sizeof(double));
  ar = f->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    w->data[i25] = 6.2831853071795862 * f->data[i25];
  }

  emxInit_real_T(&d_y, 2);
  i25 = d_y->size[0] * d_y->size[1];
  d_y->size[0] = w->size[0];
  d_y->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
  ar = w->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    ixstart = t->size[0];
    for (i26 = 0; i26 < ixstart; i26++) {
      d_y->data[i25 + d_y->size[0] * i26] = 2.0 * w->data[i25] * t->data[i26];
    }
  }

  i25 = d_y->size[0] * d_y->size[1];
  for (k = 0; k < i25; k++) {
    d_y->data[k] = sin(d_y->data[k]);
  }

  emxInit_real_T(&e_y, 2);
  i25 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = w->size[0];
  e_y->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)e_y, i25, (int)sizeof(double));
  ar = w->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    ixstart = t->size[0];
    for (i26 = 0; i26 < ixstart; i26++) {
      e_y->data[i25 + e_y->size[0] * i26] = 2.0 * w->data[i25] * t->data[i26];
    }
  }

  i25 = e_y->size[0] * e_y->size[1];
  for (k = 0; k < i25; k++) {
    e_y->data[k] = cos(e_y->data[k]);
  }

  b_emxInit_real_T(&r27, 1);
  b_emxInit_real_T(&r28, 1);
  b_emxInit_real_T(&r29, 1);
  b_emxInit_real_T(&r30, 1);
  c_sum(d_y, r27);
  c_sum(e_y, r28);
  b_atan2(r27, r28, r29);
  i25 = r30->size[0];
  r30->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)r30, i25, (int)sizeof(double));
  ar = w->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    r30->data[i25] = 2.0 * w->data[i25];
  }

  b_emxInit_real_T(&tau, 1);
  b_emxInit_real_T(&b_w, 1);
  d_rdivide(r29, r30, tau);

  /* spectral power */
  i25 = b_w->size[0];
  b_w->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)b_w, i25, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&r30);
  for (i25 = 0; i25 < ar; i25++) {
    b_w->data[i25] = w->data[i25] * tau->data[i25];
  }

  emxInit_real_T(&cterm, 2);
  c_repmat(b_w, t->size[0], e_y);
  i25 = cterm->size[0] * cterm->size[1];
  cterm->size[0] = w->size[0];
  cterm->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)cterm, i25, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&b_w);
  for (i25 = 0; i25 < ar; i25++) {
    ixstart = t->size[0];
    for (i26 = 0; i26 < ixstart; i26++) {
      T = w->data[i25] * t->data[i26];
      cterm->data[i25 + cterm->size[0] * i26] = T - e_y->data[i25 + e_y->size[0]
        * i26];
    }
  }

  i25 = cterm->size[0] * cterm->size[1];
  for (k = 0; k < i25; k++) {
    cterm->data[k] = cos(cterm->data[k]);
  }

  b_emxInit_real_T(&c_w, 1);
  i25 = c_w->size[0];
  c_w->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)c_w, i25, (int)sizeof(double));
  ar = w->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    c_w->data[i25] = w->data[i25] * tau->data[i25];
  }

  emxInit_real_T(&sterm, 2);
  c_repmat(c_w, t->size[0], e_y);
  i25 = sterm->size[0] * sterm->size[1];
  sterm->size[0] = w->size[0];
  sterm->size[1] = t->size[0];
  emxEnsureCapacity((emxArray__common *)sterm, i25, (int)sizeof(double));
  ar = w->size[0];
  emxFree_real_T(&c_w);
  for (i25 = 0; i25 < ar; i25++) {
    ixstart = t->size[0];
    for (i26 = 0; i26 < ixstart; i26++) {
      T = w->data[i25] * t->data[i26];
      sterm->data[i25 + sterm->size[0] * i26] = T - e_y->data[i25 + e_y->size[0]
        * i26];
    }
  }

  i25 = sterm->size[0] * sterm->size[1];
  for (k = 0; k < i25; k++) {
    sterm->data[k] = sin(sterm->data[k]);
  }

  i25 = tau->size[0];
  tau->size[0] = h->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i25, (int)sizeof(double));
  ar = h->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    tau->data[i25] = h->data[i25] - mu;
  }

  ixstart = tau->size[0];
  ix = tau->size[0];
  i25 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)e_y, i25, (int)sizeof(double));
  i25 = e_y->size[0] * e_y->size[1];
  e_y->size[1] = ix;
  emxEnsureCapacity((emxArray__common *)e_y, i25, (int)sizeof(double));
  ar = ixstart * ix;
  for (i25 = 0; i25 < ar; i25++) {
    e_y->data[i25] = 0.0;
  }

  for (ixstart = 0; ixstart + 1 <= tau->size[0]; ixstart++) {
    e_y->data[ixstart + e_y->size[0] * ixstart] = tau->data[ixstart];
  }

  if ((cterm->size[1] == 1) || (e_y->size[0] == 1)) {
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = cterm->size[0];
    d_y->size[1] = e_y->size[1];
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    ar = cterm->size[0];
    for (i25 = 0; i25 < ar; i25++) {
      ixstart = e_y->size[1];
      for (i26 = 0; i26 < ixstart; i26++) {
        d_y->data[i25 + d_y->size[0] * i26] = 0.0;
        ix = cterm->size[1];
        for (n = 0; n < ix; n++) {
          d_y->data[i25 + d_y->size[0] * i26] += cterm->data[i25 + cterm->size[0]
            * n] * e_y->data[n + e_y->size[0] * i26];
        }
      }
    }
  } else {
    k = cterm->size[1];
    unnamed_idx_0 = (unsigned int)cterm->size[0];
    unnamed_idx_1 = (unsigned int)e_y->size[1];
    m = cterm->size[0];
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    ar = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i25 = 0; i25 < ar; i25++) {
      d_y->data[i25] = 0.0;
    }

    if ((cterm->size[0] == 0) || (e_y->size[1] == 0)) {
    } else {
      ixstart = cterm->size[0] * (e_y->size[1] - 1);
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        i25 = ix + m;
        for (ic = ix; ic + 1 <= i25; ic++) {
          d_y->data[ic] = 0.0;
        }

        ix += m;
      }

      n = 0;
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        ar = 0;
        i25 = n + k;
        for (ib = n; ib + 1 <= i25; ib++) {
          if (e_y->data[ib] != 0.0) {
            ia = ar;
            i26 = ix + m;
            for (ic = ix; ic + 1 <= i26; ic++) {
              ia++;
              d_y->data[ic] += e_y->data[ib] * cterm->data[ia - 1];
            }
          }

          ar += m;
        }

        n += k;
        ix += m;
      }
    }
  }

  c_sum(d_y, r27);
  b_power(r27, r28);
  c_power(cterm, e_y);
  c_sum(e_y, r27);
  d_rdivide(r28, r27, r29);
  i25 = tau->size[0];
  tau->size[0] = h->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i25, (int)sizeof(double));
  ar = h->size[0];
  emxFree_real_T(&cterm);
  for (i25 = 0; i25 < ar; i25++) {
    tau->data[i25] = h->data[i25] - mu;
  }

  ixstart = tau->size[0];
  ix = tau->size[0];
  i25 = e_y->size[0] * e_y->size[1];
  e_y->size[0] = ixstart;
  emxEnsureCapacity((emxArray__common *)e_y, i25, (int)sizeof(double));
  i25 = e_y->size[0] * e_y->size[1];
  e_y->size[1] = ix;
  emxEnsureCapacity((emxArray__common *)e_y, i25, (int)sizeof(double));
  ar = ixstart * ix;
  for (i25 = 0; i25 < ar; i25++) {
    e_y->data[i25] = 0.0;
  }

  for (ixstart = 0; ixstart + 1 <= tau->size[0]; ixstart++) {
    e_y->data[ixstart + e_y->size[0] * ixstart] = tau->data[ixstart];
  }

  if ((sterm->size[1] == 1) || (e_y->size[0] == 1)) {
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = sterm->size[0];
    d_y->size[1] = e_y->size[1];
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    ar = sterm->size[0];
    for (i25 = 0; i25 < ar; i25++) {
      ixstart = e_y->size[1];
      for (i26 = 0; i26 < ixstart; i26++) {
        d_y->data[i25 + d_y->size[0] * i26] = 0.0;
        ix = sterm->size[1];
        for (n = 0; n < ix; n++) {
          d_y->data[i25 + d_y->size[0] * i26] += sterm->data[i25 + sterm->size[0]
            * n] * e_y->data[n + e_y->size[0] * i26];
        }
      }
    }
  } else {
    k = sterm->size[1];
    unnamed_idx_0 = (unsigned int)sterm->size[0];
    unnamed_idx_1 = (unsigned int)e_y->size[1];
    m = sterm->size[0];
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    i25 = d_y->size[0] * d_y->size[1];
    d_y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)d_y, i25, (int)sizeof(double));
    ar = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i25 = 0; i25 < ar; i25++) {
      d_y->data[i25] = 0.0;
    }

    if ((sterm->size[0] == 0) || (e_y->size[1] == 0)) {
    } else {
      ixstart = sterm->size[0] * (e_y->size[1] - 1);
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        i25 = ix + m;
        for (ic = ix; ic + 1 <= i25; ic++) {
          d_y->data[ic] = 0.0;
        }

        ix += m;
      }

      n = 0;
      ix = 0;
      while ((m > 0) && (ix <= ixstart)) {
        ar = 0;
        i25 = n + k;
        for (ib = n; ib + 1 <= i25; ib++) {
          if (e_y->data[ib] != 0.0) {
            ia = ar;
            i26 = ix + m;
            for (ic = ix; ic + 1 <= i26; ic++) {
              ia++;
              d_y->data[ic] += e_y->data[ib] * sterm->data[ia - 1];
            }
          }

          ar += m;
        }

        n += k;
        ix += m;
      }
    }
  }

  b_emxInit_real_T(&r31, 1);
  c_sum(d_y, r27);
  b_power(r27, r28);
  c_power(sterm, e_y);
  c_sum(e_y, r27);
  d_rdivide(r28, r27, tau);
  i25 = r31->size[0];
  r31->size[0] = r29->size[0];
  emxEnsureCapacity((emxArray__common *)r31, i25, (int)sizeof(double));
  ar = r29->size[0];
  emxFree_real_T(&r28);
  emxFree_real_T(&e_y);
  emxFree_real_T(&d_y);
  emxFree_real_T(&sterm);
  for (i25 = 0; i25 < ar; i25++) {
    r31->data[i25] = r29->data[i25] + tau->data[i25];
  }

  emxFree_real_T(&r29);
  rdivide(r31, 2.0 * s2, P);

  /* estimate of the number of independent frequencies */
  T = 2.0 * (double)f->size[0] / ofac;

  /* statistical significane of power */
  i25 = tau->size[0];
  tau->size[0] = P->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i25, (int)sizeof(double));
  ar = P->size[0];
  emxFree_real_T(&r31);
  for (i25 = 0; i25 < ar; i25++) {
    tau->data[i25] = -P->data[i25];
  }

  i25 = prob->size[0];
  prob->size[0] = tau->size[0];
  emxEnsureCapacity((emxArray__common *)prob, i25, (int)sizeof(double));
  ar = tau->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    prob->data[i25] = tau->data[i25];
  }

  for (k = 0; k < tau->size[0]; k++) {
    prob->data[k] = exp(prob->data[k]);
  }

  i25 = prob->size[0];
  emxEnsureCapacity((emxArray__common *)prob, i25, (int)sizeof(double));
  ar = prob->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    prob->data[i25] *= T;
  }

  emxInit_boolean_T(&inds, 1);
  i25 = inds->size[0];
  inds->size[0] = prob->size[0];
  emxEnsureCapacity((emxArray__common *)inds, i25, (int)sizeof(boolean_T));
  ar = prob->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    inds->data[i25] = (prob->data[i25] > 0.01);
  }

  emxInit_int32_T(&r32, 1);
  emxInit_int32_T(&r33, 1);
  eml_li_find(inds, r32);
  eml_li_find(inds, r33);
  i25 = tau->size[0];
  tau->size[0] = r33->size[0];
  emxEnsureCapacity((emxArray__common *)tau, i25, (int)sizeof(double));
  ar = r33->size[0];
  emxFree_boolean_T(&inds);
  for (i25 = 0; i25 < ar; i25++) {
    tau->data[i25] = -P->data[r33->data[i25] - 1];
  }

  emxFree_int32_T(&r33);
  i25 = w->size[0];
  w->size[0] = tau->size[0];
  emxEnsureCapacity((emxArray__common *)w, i25, (int)sizeof(double));
  ar = tau->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    w->data[i25] = tau->data[i25];
  }

  for (k = 0; k < tau->size[0]; k++) {
    w->data[k] = exp(w->data[k]);
  }

  emxFree_real_T(&tau);
  b_emxInit_real_T(&r34, 1);
  i25 = r34->size[0];
  r34->size[0] = w->size[0];
  emxEnsureCapacity((emxArray__common *)r34, i25, (int)sizeof(double));
  ar = w->size[0];
  for (i25 = 0; i25 < ar; i25++) {
    r34->data[i25] = 1.0 - w->data[i25];
  }

  emxFree_real_T(&w);
  d_power(r34, T, r27);
  ar = r27->size[0];
  emxFree_real_T(&r34);
  for (i25 = 0; i25 < ar; i25++) {
    prob->data[r32->data[i25] - 1] = 1.0 - r27->data[i25];
  }

  emxFree_real_T(&r27);
  emxFree_int32_T(&r32);
}

/* End of code generation (lomb.c) */
