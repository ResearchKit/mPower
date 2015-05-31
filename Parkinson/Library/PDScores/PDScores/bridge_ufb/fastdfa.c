/*
 * fastdfa.c
 *
 * Code generation for function 'fastdfa'
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
#include "polyfit.h"
#include "log10.h"
#include "log2.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Declarations */
static boolean_T eml_sort_le(const emxArray_real_T *v, int irow1, int irow2);

/* Function Definitions */
static boolean_T eml_sort_le(const emxArray_real_T *v, int irow1, int irow2)
{
  boolean_T p;
  boolean_T b0;
  p = true;
  if ((v->data[irow1 - 1] == v->data[irow2 - 1]) || (rtIsNaN(v->data[irow1 - 1])
       && rtIsNaN(v->data[irow2 - 1]))) {
    b0 = true;
  } else {
    b0 = false;
  }

  if (!b0) {
    if ((v->data[irow1 - 1] <= v->data[irow2 - 1]) || rtIsNaN(v->data[irow2 - 1]))
    {
      p = true;
    } else {
      p = false;
    }
  }

  return p;
}

void fastdfa(const emxArray_real_T *x, double *alpha, emxArray_real_T *intervals,
             emxArray_real_T *flucts)
{
  double nscales;
  emxArray_real_T *xpts;
  int pEnd;
  int loop_ub;
  emxArray_real_T *ypts;
  emxArray_real_T *ycol;
  emxArray_real_T *y;
  int i2;
  emxArray_int32_T *idx;
  int n;
  emxArray_int32_T *idx0;
  int i;
  int j;
  int p;
  int q;
  int qEnd;
  int kEnd;
  emxArray_real_T *datapts;
  double coeffs[2];

  /*  Performs fast detrended fluctuation analysis on a nonstationary input signal to */
  /*  obtain an estimate for the scaling exponent. */
  /*  */
  /*  Useage: */
  /*  [alpha, intervals, flucts] = fastdfa(x) */
  /*  [alpha, intervals, flucts] = fastdfa(x, intervals) */
  /*  Inputs */
  /*     x          - input signal: must be a row vector */
  /*  Optional inputs */
  /*     intervals  - List of sample interval widths at each scale */
  /*                  (If not specified, then a binary subdivision is constructed) */
  /*  */
  /*  Outputs: */
  /*     alpha      - Estimated scaling exponent */
  /*     intervals  - List of sample interval widths at each scale */
  /*     flucts     - List of fluctuation amplitudes at each scale */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2006-2014. */
  /*  If you use this code for academic publication, please cite: */
  /*  M. Little, P. McSharry, I. Moroz, S. Roberts (2006), */
  /*  Nonlinear, biophysically-informed speech pathology detection */
  /*  in Proceedings of ICASSP 2006, IEEE Publishers: Toulouse, France. */
  nscales = floor(scalar_log2(x->size[0]));
  if (rt_powd_snf(2.0, nscales - 1.0) > (double)x->size[0] / 2.5) {
    nscales--;
  }

  b_emxInit_real_T(&xpts, 1);
  pEnd = xpts->size[0];
  xpts->size[0] = (int)nscales;
  emxEnsureCapacity((emxArray__common *)xpts, pEnd, (int)sizeof(double));
  loop_ub = (int)nscales;
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    xpts->data[pEnd] = 0.0;
  }

  b_emxInit_real_T(&ypts, 1);
  pEnd = ypts->size[0];
  ypts->size[0] = (int)nscales;
  emxEnsureCapacity((emxArray__common *)ypts, pEnd, (int)sizeof(double));
  loop_ub = (int)nscales;
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    ypts->data[pEnd] = 0.0;
  }

  b_emxInit_real_T(&ycol, 1);
  pEnd = ycol->size[0];
  ycol->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)ycol, pEnd, (int)sizeof(double));
  loop_ub = x->size[0];
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    ycol->data[pEnd] = x->data[pEnd];
  }

  emxInit_real_T(&y, 2);
  fastdfa_core_nomex(&xpts->data[0], &ypts->data[0], &ycol->data[0], (double)
                     x->size[0], 1.0);

  /*  Sort the intervals, and produce a log-log straight line fit */
  loop_ub = xpts->size[0];
  i2 = ypts->size[0];
  pEnd = y->size[0] * y->size[1];
  y->size[0] = loop_ub;
  y->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)y, pEnd, (int)sizeof(double));
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    y->data[pEnd] = xpts->data[pEnd];
  }

  for (pEnd = 0; pEnd < i2; pEnd++) {
    y->data[pEnd + y->size[0]] = ypts->data[pEnd];
  }

  emxInit_int32_T(&idx, 1);
  n = y->size[0];
  pEnd = idx->size[0];
  idx->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)idx, pEnd, (int)sizeof(int));
  if (y->size[0] == 0) {
    for (loop_ub = 1; loop_ub <= n; loop_ub++) {
      idx->data[loop_ub - 1] = loop_ub;
    }
  } else {
    for (loop_ub = 1; loop_ub <= n; loop_ub++) {
      idx->data[loop_ub - 1] = loop_ub;
    }

    for (loop_ub = 1; loop_ub <= n - 1; loop_ub += 2) {
      if (eml_sort_le(y, loop_ub, loop_ub + 1)) {
      } else {
        idx->data[loop_ub - 1] = loop_ub + 1;
        idx->data[loop_ub] = loop_ub;
      }
    }

    emxInit_int32_T(&idx0, 1);
    pEnd = idx0->size[0];
    idx0->size[0] = y->size[0];
    emxEnsureCapacity((emxArray__common *)idx0, pEnd, (int)sizeof(int));
    loop_ub = y->size[0];
    for (pEnd = 0; pEnd < loop_ub; pEnd++) {
      idx0->data[pEnd] = 1;
    }

    i = 2;
    while (i < n) {
      i2 = i << 1;
      j = 1;
      for (pEnd = 1 + i; pEnd < n + 1; pEnd = qEnd + i) {
        p = j;
        q = pEnd;
        qEnd = j + i2;
        if (qEnd > n + 1) {
          qEnd = n + 1;
        }

        loop_ub = 0;
        kEnd = qEnd - j;
        while (loop_ub + 1 <= kEnd) {
          if (eml_sort_le(y, idx->data[p - 1], idx->data[q - 1])) {
            idx0->data[loop_ub] = idx->data[p - 1];
            p++;
            if (p == pEnd) {
              while (q < qEnd) {
                loop_ub++;
                idx0->data[loop_ub] = idx->data[q - 1];
                q++;
              }
            }
          } else {
            idx0->data[loop_ub] = idx->data[q - 1];
            q++;
            if (q == qEnd) {
              while (p < pEnd) {
                loop_ub++;
                idx0->data[loop_ub] = idx->data[p - 1];
                p++;
              }
            }
          }

          loop_ub++;
        }

        for (loop_ub = 0; loop_ub + 1 <= kEnd; loop_ub++) {
          idx->data[(j + loop_ub) - 1] = idx0->data[loop_ub];
        }

        j = qEnd;
      }

      i = i2;
    }

    emxFree_int32_T(&idx0);
  }

  emxInit_real_T(&datapts, 2);
  pEnd = datapts->size[0] * datapts->size[1];
  datapts->size[0] = y->size[0];
  datapts->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)datapts, pEnd, (int)sizeof(double));
  loop_ub = y->size[0] * y->size[1];
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    datapts->data[pEnd] = y->data[pEnd];
  }

  i2 = y->size[0];
  loop_ub = y->size[0];
  pEnd = ycol->size[0];
  ycol->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)ycol, pEnd, (int)sizeof(double));
  emxFree_real_T(&y);
  for (j = 0; j < 2; j++) {
    for (i = 0; i + 1 <= i2; i++) {
      ycol->data[i] = datapts->data[(idx->data[i] + datapts->size[0] * j) - 1];
    }

    for (i = 0; i + 1 <= i2; i++) {
      datapts->data[i + datapts->size[0] * j] = ycol->data[i];
    }
  }

  emxFree_real_T(&ycol);
  emxFree_int32_T(&idx);
  loop_ub = datapts->size[0];
  pEnd = intervals->size[0];
  intervals->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)intervals, pEnd, (int)sizeof(double));
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    intervals->data[pEnd] = datapts->data[pEnd];
  }

  loop_ub = datapts->size[0];
  pEnd = flucts->size[0];
  flucts->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)flucts, pEnd, (int)sizeof(double));
  for (pEnd = 0; pEnd < loop_ub; pEnd++) {
    flucts->data[pEnd] = datapts->data[pEnd + datapts->size[0]];
  }

  emxFree_real_T(&datapts);
  b_log10(xpts);
  b_log10(ypts);
  polyfit(xpts, ypts, coeffs);
  *alpha = coeffs[0];
  emxFree_real_T(&ypts);
  emxFree_real_T(&xpts);
}

/* End of code generation (fastdfa.c) */
