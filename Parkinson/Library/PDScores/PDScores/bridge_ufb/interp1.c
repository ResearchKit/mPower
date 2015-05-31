/*
 * interp1.c
 *
 * Code generation for function 'interp1'
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
#include "interp1.h"
#include "bridge_ufb_emxutil.h"
#include "flip.h"
#include "spline.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
               *varargin_2, const emxArray_real_T *varargin_3, emxArray_real_T
               *Vq)
{
  emxArray_real_T *y;
  int high_i;
  int low_ip1;
  emxArray_real_T *x;
  int nyrows;
  int nycols;
  int nx;
  unsigned int outsize_idx_0;
  unsigned int outsize_idx_1;
  int k;
  int32_T exitg1;
  double r;
  int nxi;
  int low_i;
  emxInit_real_T(&y, 2);
  high_i = y->size[0] * y->size[1];
  y->size[0] = varargin_2->size[0];
  y->size[1] = varargin_2->size[1];
  emxEnsureCapacity((emxArray__common *)y, high_i, (int)sizeof(double));
  low_ip1 = varargin_2->size[0] * varargin_2->size[1];
  for (high_i = 0; high_i < low_ip1; high_i++) {
    y->data[high_i] = varargin_2->data[high_i];
  }

  b_emxInit_real_T(&x, 1);
  high_i = x->size[0];
  x->size[0] = varargin_1->size[0];
  emxEnsureCapacity((emxArray__common *)x, high_i, (int)sizeof(double));
  low_ip1 = varargin_1->size[0];
  for (high_i = 0; high_i < low_ip1; high_i++) {
    x->data[high_i] = varargin_1->data[high_i];
  }

  nyrows = varargin_2->size[0];
  nycols = varargin_2->size[1] - 1;
  nx = varargin_1->size[0];
  outsize_idx_0 = (unsigned int)varargin_3->size[0];
  outsize_idx_1 = (unsigned int)varargin_2->size[1];
  high_i = Vq->size[0] * Vq->size[1];
  Vq->size[0] = (int)outsize_idx_0;
  emxEnsureCapacity((emxArray__common *)Vq, high_i, (int)sizeof(double));
  high_i = Vq->size[0] * Vq->size[1];
  Vq->size[1] = (int)outsize_idx_1;
  emxEnsureCapacity((emxArray__common *)Vq, high_i, (int)sizeof(double));
  low_ip1 = (int)outsize_idx_0 * (int)outsize_idx_1;
  for (high_i = 0; high_i < low_ip1; high_i++) {
    Vq->data[high_i] = rtNaN;
  }

  if (varargin_3->size[0] == 0) {
  } else {
    k = 1;
    do {
      exitg1 = 0;
      if (k <= nx) {
        if (rtIsNaN(varargin_1->data[k - 1])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (varargin_1->data[1] < varargin_1->data[0]) {
          high_i = nx >> 1;
          for (low_ip1 = 1; low_ip1 <= high_i; low_ip1++) {
            r = x->data[low_ip1 - 1];
            x->data[low_ip1 - 1] = x->data[nx - low_ip1];
            x->data[nx - low_ip1] = r;
          }

          flip(y);
        }

        nxi = varargin_3->size[0];
        for (k = 0; k + 1 <= nxi; k++) {
          if (rtIsNaN(varargin_3->data[k])) {
            for (low_ip1 = 0; low_ip1 <= nycols; low_ip1++) {
              Vq->data[k + low_ip1 * nxi] = rtNaN;
            }
          } else if ((varargin_3->data[k] > x->data[x->size[0] - 1]) ||
                     (varargin_3->data[k] < x->data[0])) {
          } else {
            low_i = 1;
            low_ip1 = 2;
            high_i = x->size[0];
            while (high_i > low_ip1) {
              nx = (low_i >> 1) + (high_i >> 1);
              if (((low_i & 1) == 1) && ((high_i & 1) == 1)) {
                nx++;
              }

              if (varargin_3->data[k] >= x->data[nx - 1]) {
                low_i = nx;
                low_ip1 = nx + 1;
              } else {
                high_i = nx;
              }
            }

            r = (varargin_3->data[k] - x->data[low_i - 1]) / (x->data[low_i] -
              x->data[low_i - 1]);
            for (low_ip1 = 0; low_ip1 <= nycols; low_ip1++) {
              if (y->data[(low_i + low_ip1 * nyrows) - 1] == y->data[low_i +
                  low_ip1 * nyrows]) {
                Vq->data[k + low_ip1 * nxi] = y->data[(low_i + low_ip1 * nyrows)
                  - 1];
              } else {
                Vq->data[k + low_ip1 * nxi] = (1.0 - r) * y->data[(low_i +
                  low_ip1 * nyrows) - 1] + r * y->data[low_i + low_ip1 * nyrows];
              }
            }
          }
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  emxFree_real_T(&x);
  emxFree_real_T(&y);
}

void interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
             *varargin_2, const emxArray_real_T *varargin_3, emxArray_real_T *Vq)
{
  emxArray_real_T *y;
  int low_i;
  int mid_i;
  emxArray_real_T *x;
  int nycols;
  int nx;
  unsigned int outsize_idx_0;
  unsigned int outsize_idx_1;
  int k;
  emxArray_real_T *pp_breaks;
  emxArray_real_T *pp_coefs;
  emxArray_real_T *yit;
  emxArray_real_T *b_y;
  int32_T exitg1;
  int low_ip1;
  double xloc;
  int nxi;
  int elementsPerPage;
  int coefStride;
  int icp;
  emxInit_real_T(&y, 2);
  low_i = y->size[0] * y->size[1];
  y->size[0] = varargin_2->size[0];
  y->size[1] = varargin_2->size[1];
  emxEnsureCapacity((emxArray__common *)y, low_i, (int)sizeof(double));
  mid_i = varargin_2->size[0] * varargin_2->size[1];
  for (low_i = 0; low_i < mid_i; low_i++) {
    y->data[low_i] = varargin_2->data[low_i];
  }

  b_emxInit_real_T(&x, 1);
  low_i = x->size[0];
  x->size[0] = varargin_1->size[0];
  emxEnsureCapacity((emxArray__common *)x, low_i, (int)sizeof(double));
  mid_i = varargin_1->size[0];
  for (low_i = 0; low_i < mid_i; low_i++) {
    x->data[low_i] = varargin_1->data[low_i];
  }

  nycols = varargin_2->size[1];
  nx = varargin_1->size[0];
  outsize_idx_0 = (unsigned int)varargin_3->size[0];
  outsize_idx_1 = (unsigned int)varargin_2->size[1];
  low_i = Vq->size[0] * Vq->size[1];
  Vq->size[0] = (int)outsize_idx_0;
  emxEnsureCapacity((emxArray__common *)Vq, low_i, (int)sizeof(double));
  low_i = Vq->size[0] * Vq->size[1];
  Vq->size[1] = (int)outsize_idx_1;
  emxEnsureCapacity((emxArray__common *)Vq, low_i, (int)sizeof(double));
  mid_i = (int)outsize_idx_0 * (int)outsize_idx_1;
  for (low_i = 0; low_i < mid_i; low_i++) {
    Vq->data[low_i] = 0.0;
  }

  if (varargin_3->size[0] == 0) {
  } else {
    k = 1;
    emxInit_real_T(&pp_breaks, 2);
    c_emxInit_real_T(&pp_coefs, 3);
    b_emxInit_real_T(&yit, 1);
    emxInit_real_T(&b_y, 2);
    do {
      exitg1 = 0;
      if (k <= nx) {
        if (rtIsNaN(varargin_1->data[k - 1])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (varargin_1->data[1] < varargin_1->data[0]) {
          low_i = nx >> 1;
          for (low_ip1 = 1; low_ip1 <= low_i; low_ip1++) {
            xloc = x->data[low_ip1 - 1];
            x->data[low_ip1 - 1] = x->data[nx - low_ip1];
            x->data[nx - low_ip1] = xloc;
          }

          flip(y);
        }

        nxi = varargin_3->size[0];
        low_i = b_y->size[0] * b_y->size[1];
        b_y->size[0] = y->size[1];
        b_y->size[1] = y->size[0];
        emxEnsureCapacity((emxArray__common *)b_y, low_i, (int)sizeof(double));
        mid_i = y->size[0];
        for (low_i = 0; low_i < mid_i; low_i++) {
          low_ip1 = y->size[1];
          for (nx = 0; nx < low_ip1; nx++) {
            b_y->data[nx + b_y->size[0] * low_i] = y->data[low_i + y->size[0] *
              nx];
          }
        }

        spline(x, b_y, pp_breaks, pp_coefs);
        for (k = 0; k + 1 <= nxi; k++) {
          if (rtIsNaN(varargin_3->data[k])) {
            for (nx = 1; nx <= nycols; nx++) {
              Vq->data[(nx - 1) * nxi + k] = rtNaN;
            }
          } else {
            if ((varargin_3->data[k] >= x->data[0]) && (varargin_3->data[k] <=
                 x->data[x->size[0] - 1])) {
              xloc = varargin_3->data[k];
              elementsPerPage = pp_coefs->size[0];
              coefStride = pp_coefs->size[0] * (pp_breaks->size[1] - 1);
              low_i = yit->size[0];
              yit->size[0] = pp_coefs->size[0];
              emxEnsureCapacity((emxArray__common *)yit, low_i, (int)sizeof
                                (double));
              if (rtIsNaN(varargin_3->data[k])) {
                for (nx = 1; nx <= elementsPerPage; nx++) {
                  yit->data[nx - 1] = xloc;
                }
              } else {
                low_i = 1;
                low_ip1 = 2;
                nx = pp_breaks->size[1];
                while (nx > low_ip1) {
                  mid_i = (low_i >> 1) + (nx >> 1);
                  if (((low_i & 1) == 1) && ((nx & 1) == 1)) {
                    mid_i++;
                  }

                  if (xloc >= pp_breaks->data[mid_i - 1]) {
                    low_i = mid_i;
                    low_ip1 = mid_i + 1;
                  } else {
                    nx = mid_i;
                  }
                }

                icp = (low_i - 1) * pp_coefs->size[0];
                xloc = varargin_3->data[k] - pp_breaks->data[low_i - 1];
                for (nx = 0; nx + 1 <= elementsPerPage; nx++) {
                  yit->data[nx] = pp_coefs->data[icp + nx];
                }

                for (mid_i = 2; mid_i <= pp_coefs->size[2]; mid_i++) {
                  low_ip1 = icp + (mid_i - 1) * coefStride;
                  for (nx = 0; nx + 1 <= elementsPerPage; nx++) {
                    yit->data[nx] = xloc * yit->data[nx] + pp_coefs->
                      data[low_ip1 + nx];
                  }
                }
              }

              for (nx = 0; nx + 1 <= nycols; nx++) {
                Vq->data[nx * nxi + k] = yit->data[nx];
              }
            }
          }
        }

        exitg1 = 1;
      }
    } while (exitg1 == 0);

    emxFree_real_T(&b_y);
    emxFree_real_T(&yit);
    emxFree_real_T(&pp_coefs);
    emxFree_real_T(&pp_breaks);
  }

  emxFree_real_T(&x);
  emxFree_real_T(&y);
}

/* End of code generation (interp1.c) */
