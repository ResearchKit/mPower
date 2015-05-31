/*
 * median.c
 *
 * Code generation for function 'median'
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
#include "median.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void eml_sort_idx(const emxArray_real_T *x, emxArray_int32_T *idx)
{
  int n;
  unsigned int unnamed_idx_0;
  int k;
  boolean_T p;
  emxArray_int32_T *idx0;
  int i;
  int i2;
  int j;
  int pEnd;
  int b_p;
  int q;
  int qEnd;
  int kEnd;
  n = x->size[0];
  unnamed_idx_0 = (unsigned int)x->size[0];
  k = idx->size[0];
  idx->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)idx, k, (int)sizeof(int));
  if (x->size[0] == 0) {
    for (k = 1; k <= n; k++) {
      idx->data[k - 1] = k;
    }
  } else {
    for (k = 1; k <= n; k++) {
      idx->data[k - 1] = k;
    }

    for (k = 1; k <= n - 1; k += 2) {
      if ((x->data[k - 1] <= x->data[k]) || rtIsNaN(x->data[k])) {
        p = true;
      } else {
        p = false;
      }

      if (p) {
      } else {
        idx->data[k - 1] = k + 1;
        idx->data[k] = k;
      }
    }

    emxInit_int32_T(&idx0, 1);
    k = idx0->size[0];
    idx0->size[0] = x->size[0];
    emxEnsureCapacity((emxArray__common *)idx0, k, (int)sizeof(int));
    i = x->size[0];
    for (k = 0; k < i; k++) {
      idx0->data[k] = 1;
    }

    i = 2;
    while (i < n) {
      i2 = i << 1;
      j = 1;
      for (pEnd = 1 + i; pEnd < n + 1; pEnd = qEnd + i) {
        b_p = j;
        q = pEnd - 1;
        qEnd = j + i2;
        if (qEnd > n + 1) {
          qEnd = n + 1;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          if ((x->data[idx->data[b_p - 1] - 1] <= x->data[idx->data[q] - 1]) ||
              rtIsNaN(x->data[idx->data[q] - 1])) {
            p = true;
          } else {
            p = false;
          }

          if (p) {
            idx0->data[k] = idx->data[b_p - 1];
            b_p++;
            if (b_p == pEnd) {
              while (q + 1 < qEnd) {
                k++;
                idx0->data[k] = idx->data[q];
                q++;
              }
            }
          } else {
            idx0->data[k] = idx->data[q];
            q++;
            if (q + 1 == qEnd) {
              while (b_p < pEnd) {
                k++;
                idx0->data[k] = idx->data[b_p - 1];
                b_p++;
              }
            }
          }

          k++;
        }

        for (k = 0; k + 1 <= kEnd; k++) {
          idx->data[(j + k) - 1] = idx0->data[k];
        }

        j = qEnd;
      }

      i = i2;
    }

    emxFree_int32_T(&idx0);
  }
}

double median(const emxArray_real_T *x)
{
  double y;
  int k;
  int midm1;
  int n;
  unsigned int uv8[2];
  emxArray_int32_T *idx;
  boolean_T p;
  emxArray_int32_T *idx0;
  int i;
  int i2;
  int j;
  int pEnd;
  int b_p;
  int q;
  int qEnd;
  int kEnd;
  if (x->size[1] == 0) {
    y = rtNaN;
  } else {
    k = x->size[1];
    midm1 = (k + (k < 0)) >> 1;
    n = x->size[1];
    for (k = 0; k < 2; k++) {
      uv8[k] = (unsigned int)x->size[k];
    }

    b_emxInit_int32_T(&idx, 2);
    k = idx->size[0] * idx->size[1];
    idx->size[0] = 1;
    idx->size[1] = (int)uv8[1];
    emxEnsureCapacity((emxArray__common *)idx, k, (int)sizeof(int));
    for (k = 1; k <= n; k++) {
      idx->data[k - 1] = k;
    }

    for (k = 1; k <= n - 1; k += 2) {
      if ((x->data[k - 1] <= x->data[k]) || rtIsNaN(x->data[k])) {
        p = true;
      } else {
        p = false;
      }

      if (p) {
      } else {
        idx->data[k - 1] = k + 1;
        idx->data[k] = k;
      }
    }

    emxInit_int32_T(&idx0, 1);
    k = idx0->size[0];
    idx0->size[0] = x->size[1];
    emxEnsureCapacity((emxArray__common *)idx0, k, (int)sizeof(int));
    i = x->size[1];
    for (k = 0; k < i; k++) {
      idx0->data[k] = 1;
    }

    i = 2;
    while (i < n) {
      i2 = i << 1;
      j = 1;
      for (pEnd = 1 + i; pEnd < n + 1; pEnd = qEnd + i) {
        b_p = j;
        q = pEnd - 1;
        qEnd = j + i2;
        if (qEnd > n + 1) {
          qEnd = n + 1;
        }

        k = 0;
        kEnd = qEnd - j;
        while (k + 1 <= kEnd) {
          if ((x->data[idx->data[b_p - 1] - 1] <= x->data[idx->data[q] - 1]) ||
              rtIsNaN(x->data[idx->data[q] - 1])) {
            p = true;
          } else {
            p = false;
          }

          if (p) {
            idx0->data[k] = idx->data[b_p - 1];
            b_p++;
            if (b_p == pEnd) {
              while (q + 1 < qEnd) {
                k++;
                idx0->data[k] = idx->data[q];
                q++;
              }
            }
          } else {
            idx0->data[k] = idx->data[q];
            q++;
            if (q + 1 == qEnd) {
              while (b_p < pEnd) {
                k++;
                idx0->data[k] = idx->data[b_p - 1];
                b_p++;
              }
            }
          }

          k++;
        }

        for (k = 0; k + 1 <= kEnd; k++) {
          idx->data[(j + k) - 1] = idx0->data[k];
        }

        j = qEnd;
      }

      i = i2;
    }

    emxFree_int32_T(&idx0);
    if (rtIsNaN(x->data[idx->data[idx->size[1] - 1] - 1])) {
      y = x->data[idx->data[idx->size[1] - 1] - 1];
    } else if (midm1 << 1 == x->size[1]) {
      if (((x->data[idx->data[midm1 - 1] - 1] < 0.0) && (x->data[idx->data[midm1]
            - 1] >= 0.0)) || rtIsInf(x->data[idx->data[midm1 - 1] - 1]) ||
          rtIsInf(x->data[idx->data[midm1] - 1])) {
        y = (x->data[idx->data[midm1 - 1] - 1] + x->data[idx->data[midm1] - 1]) /
          2.0;
      } else {
        y = x->data[idx->data[midm1 - 1] - 1] + (x->data[idx->data[midm1] - 1] -
          x->data[idx->data[midm1 - 1] - 1]) / 2.0;
      }
    } else {
      y = x->data[idx->data[midm1] - 1];
    }

    emxFree_int32_T(&idx);
  }

  return y;
}

double vectormedian(const emxArray_real_T *v)
{
  double m;
  emxArray_int32_T *idx;
  int midm1;
  emxInit_int32_T(&idx, 1);
  midm1 = v->size[0];
  midm1 = (midm1 + (midm1 < 0)) >> 1;
  eml_sort_idx(v, idx);
  if (rtIsNaN(v->data[idx->data[idx->size[0] - 1] - 1])) {
    m = v->data[idx->data[idx->size[0] - 1] - 1];
  } else if (midm1 << 1 == v->size[0]) {
    if (((v->data[idx->data[midm1 - 1] - 1] < 0.0) && (v->data[idx->data[midm1]
          - 1] >= 0.0)) || rtIsInf(v->data[idx->data[midm1 - 1] - 1]) || rtIsInf
        (v->data[idx->data[midm1] - 1])) {
      m = (v->data[idx->data[midm1 - 1] - 1] + v->data[idx->data[midm1] - 1]) /
        2.0;
    } else {
      m = v->data[idx->data[midm1 - 1] - 1] + (v->data[idx->data[midm1] - 1] -
        v->data[idx->data[midm1 - 1] - 1]) / 2.0;
    }
  } else {
    m = v->data[idx->data[midm1] - 1];
  }

  emxFree_int32_T(&idx);
  return m;
}

/* End of code generation (median.c) */
