/*
 * polyfit.c
 *
 * Code generation for function 'polyfit'
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
#include "polyfit.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Declarations */
static void b_eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B, double
  Y[3], double *rankR);
static double b_eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void b_eml_xscal(int n, double a, emxArray_real_T *x, int ix0);
static double c_eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static double d_eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static double eml_matlab_zlarfg(void);
static void eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B, double Y[2],
  double *rankR);
static double eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void eml_xscal(int n, double a, emxArray_real_T *x, int ix0);

/* Function Definitions */
static void b_eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B, double
  Y[3], double *rankR)
{
  emxArray_real_T *b_A;
  int m;
  int mn;
  int i23;
  int itemp;
  int b_m;
  int b_mn;
  double tau_data[3];
  signed char jpvt[3];
  double work[3];
  int i;
  int k;
  double vn1[3];
  double vn2[3];
  int iy;
  double smax;
  int i_i;
  int mmi;
  int ix;
  double temp2;
  int pvt;
  double atmp;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int32_T exitg1;
  double t;
  unsigned int b_i;
  emxInit_real_T(&b_A, 2);
  m = A->size[0] - 2;
  mn = (int)fmin(A->size[0], 3.0) - 1;
  i23 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b_A, i23, (int)sizeof(double));
  itemp = A->size[0] * A->size[1];
  for (i23 = 0; i23 < itemp; i23++) {
    b_A->data[i23] = A->data[i23];
  }

  b_m = b_A->size[0];
  if (b_A->size[0] <= 3) {
    b_mn = b_A->size[0];
  } else {
    b_mn = 3;
  }

  for (i23 = 0; i23 < 3; i23++) {
    jpvt[i23] = (signed char)(1 + i23);
  }

  if (b_A->size[0] == 0) {
  } else {
    for (i = 0; i < 3; i++) {
      work[i] = 0.0;
    }

    k = 1;
    for (iy = 0; iy < 3; iy++) {
      smax = c_eml_xnrm2(b_m, b_A, k);
      vn2[iy] = smax;
      k += b_m;
      vn1[iy] = smax;
    }

    for (i = 0; i + 1 <= b_mn; i++) {
      i_i = i + i * b_m;
      mmi = (b_m - i) - 1;
      itemp = 1;
      if (3 - i > 1) {
        ix = i;
        smax = fabs(vn1[i]);
        for (k = 2; k <= 3 - i; k++) {
          ix++;
          temp2 = fabs(vn1[ix]);
          if (temp2 > smax) {
            itemp = k;
            smax = temp2;
          }
        }
      }

      pvt = (i + itemp) - 1;
      if (pvt + 1 != i + 1) {
        ix = b_m * pvt;
        iy = b_m * i;
        for (k = 1; k <= b_m; k++) {
          smax = b_A->data[ix];
          b_A->data[ix] = b_A->data[iy];
          b_A->data[iy] = smax;
          ix++;
          iy++;
        }

        itemp = jpvt[pvt];
        jpvt[pvt] = jpvt[i];
        jpvt[i] = (signed char)itemp;
        vn1[pvt] = vn1[i];
        vn2[pvt] = vn2[i];
      }

      if (i + 1 < b_m) {
        atmp = b_A->data[i_i];
        temp2 = 0.0;
        if (1 + mmi <= 0) {
        } else {
          smax = d_eml_xnrm2(mmi, b_A, i_i + 2);
          if (smax != 0.0) {
            smax = hypot(b_A->data[i_i], smax);
            if (b_A->data[i_i] >= 0.0) {
              smax = -smax;
            }

            if (fabs(smax) < 1.0020841800044864E-292) {
              itemp = 0;
              do {
                itemp++;
                b_eml_xscal(mmi, 9.9792015476736E+291, b_A, i_i + 2);
                smax *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(fabs(smax) >= 1.0020841800044864E-292));

              smax = d_eml_xnrm2(mmi, b_A, i_i + 2);
              smax = hypot(atmp, smax);
              if (atmp >= 0.0) {
                smax = -smax;
              }

              temp2 = (smax - atmp) / smax;
              b_eml_xscal(mmi, 1.0 / (atmp - smax), b_A, i_i + 2);
              for (k = 1; k <= itemp; k++) {
                smax *= 1.0020841800044864E-292;
              }

              atmp = smax;
            } else {
              temp2 = (smax - b_A->data[i_i]) / smax;
              b_eml_xscal(mmi, 1.0 / (b_A->data[i_i] - smax), b_A, i_i + 2);
              atmp = smax;
            }
          }
        }

        tau_data[i] = temp2;
      } else {
        atmp = b_A->data[i_i];
        tau_data[i] = eml_matlab_zlarfg();
      }

      b_A->data[i_i] = atmp;
      if (i + 1 < 3) {
        atmp = b_A->data[i_i];
        b_A->data[i_i] = 1.0;
        i_ip1 = (i + (i + 1) * b_m) + 1;
        if (tau_data[i] != 0.0) {
          lastv = mmi;
          itemp = i_i + mmi;
          while ((lastv + 1 > 0) && (b_A->data[itemp] == 0.0)) {
            lastv--;
            itemp--;
          }

          lastc = 2 - i;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            itemp = i_ip1 + (lastc - 1) * b_m;
            k = itemp;
            do {
              exitg1 = 0;
              if (k <= itemp + lastv) {
                if (b_A->data[k - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  k++;
                }
              } else {
                lastc--;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = -1;
          lastc = 0;
        }

        if (lastv + 1 > 0) {
          if (lastc == 0) {
          } else {
            for (iy = 1; iy <= lastc; iy++) {
              work[iy - 1] = 0.0;
            }

            iy = 0;
            i23 = i_ip1 + b_m * (lastc - 1);
            itemp = i_ip1;
            while ((b_m > 0) && (itemp <= i23)) {
              ix = i_i;
              smax = 0.0;
              pvt = itemp + lastv;
              for (k = itemp; k <= pvt; k++) {
                smax += b_A->data[k - 1] * b_A->data[ix];
                ix++;
              }

              work[iy] += smax;
              iy++;
              itemp += b_m;
            }
          }

          if (-tau_data[i] == 0.0) {
          } else {
            itemp = 0;
            for (iy = 1; iy <= lastc; iy++) {
              if (work[itemp] != 0.0) {
                smax = work[itemp] * -tau_data[i];
                ix = i_i;
                i23 = lastv + i_ip1;
                for (pvt = i_ip1; pvt <= i23; pvt++) {
                  b_A->data[pvt - 1] += b_A->data[ix] * smax;
                  ix++;
                }
              }

              itemp++;
              i_ip1 += b_m;
            }
          }
        }

        b_A->data[i_i] = atmp;
      }

      for (iy = i + 1; iy + 1 < 4; iy++) {
        itemp = (i + b_m * iy) + 1;
        if (vn1[iy] != 0.0) {
          smax = fabs(b_A->data[i + b_A->size[0] * iy]) / vn1[iy];
          smax = 1.0 - smax * smax;
          if (smax < 0.0) {
            smax = 0.0;
          }

          temp2 = vn1[iy] / vn2[iy];
          temp2 = smax * (temp2 * temp2);
          if (temp2 <= 1.4901161193847656E-8) {
            if (i + 1 < b_m) {
              smax = 0.0;
              if (mmi < 1) {
              } else if (mmi == 1) {
                smax = fabs(b_A->data[itemp]);
              } else {
                temp2 = 2.2250738585072014E-308;
                pvt = itemp + mmi;
                while (itemp + 1 <= pvt) {
                  atmp = fabs(b_A->data[itemp]);
                  if (atmp > temp2) {
                    t = temp2 / atmp;
                    smax = 1.0 + smax * t * t;
                    temp2 = atmp;
                  } else {
                    t = atmp / temp2;
                    smax += t * t;
                  }

                  itemp++;
                }

                smax = temp2 * sqrt(smax);
              }

              vn1[iy] = smax;
              vn2[iy] = vn1[iy];
            } else {
              vn1[iy] = 0.0;
              vn2[iy] = 0.0;
            }
          } else {
            vn1[iy] *= sqrt(smax);
          }
        }
      }
    }
  }

  *rankR = 0.0;
  if (mn + 1 > 0) {
    smax = fmax(A->size[0], 3.0) * fabs(b_A->data[0]) * 2.2204460492503131E-16;
    k = 0;
    while ((k <= mn) && (!(fabs(b_A->data[k + b_A->size[0] * k]) <= smax))) {
      (*rankR)++;
      k++;
    }
  }

  for (i = 0; i < 3; i++) {
    Y[i] = 0.0;
  }

  for (iy = 0; iy <= mn; iy++) {
    if (tau_data[iy] != 0.0) {
      smax = B->data[iy];
      i23 = m - iy;
      for (i = 0; i <= i23; i++) {
        b_i = ((unsigned int)iy + i) + 2U;
        smax += b_A->data[((int)b_i + b_A->size[0] * iy) - 1] * B->data[(int)b_i
          - 1];
      }

      smax *= tau_data[iy];
      if (smax != 0.0) {
        B->data[iy] -= smax;
        i23 = m - iy;
        for (i = 0; i <= i23; i++) {
          b_i = ((unsigned int)iy + i) + 2U;
          B->data[(int)b_i - 1] -= b_A->data[((int)b_i + b_A->size[0] * iy) - 1]
            * smax;
        }
      }
    }
  }

  for (i = 0; i <= mn; i++) {
    Y[jpvt[i] - 1] = B->data[i];
  }

  for (iy = 0; iy <= mn; iy++) {
    itemp = mn - iy;
    Y[jpvt[itemp] - 1] /= b_A->data[itemp + b_A->size[0] * itemp];
    for (i = 0; i < itemp; i++) {
      Y[jpvt[i] - 1] -= Y[jpvt[itemp] - 1] * b_A->data[i + b_A->size[0] * itemp];
    }
  }

  emxFree_real_T(&b_A);
}

static double b_eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

static void b_eml_xscal(int n, double a, emxArray_real_T *x, int ix0)
{
  int i32;
  int k;
  i32 = (ix0 + n) - 1;
  for (k = ix0; k <= i32; k++) {
    x->data[k - 1] *= a;
  }
}

static double c_eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

static double d_eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

static double eml_matlab_zlarfg(void)
{
  return 0.0;
}

static void eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B, double Y[2],
  double *rankR)
{
  emxArray_real_T *b_A;
  int m;
  int mn;
  int i6;
  int itemp;
  int b_m;
  int b_mn;
  double tau_data[2];
  signed char jpvt[2];
  double work[2];
  int i;
  int k;
  double vn1[2];
  double vn2[2];
  int pvt;
  double smax;
  int i_i;
  int mmi;
  double temp2;
  int ix;
  int iy;
  double atmp;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int32_T exitg1;
  double t;
  unsigned int b_i;
  emxInit_real_T(&b_A, 2);
  m = A->size[0] - 2;
  mn = (int)fmin(A->size[0], 2.0) - 1;
  i6 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)b_A, i6, (int)sizeof(double));
  itemp = A->size[0] * A->size[1];
  for (i6 = 0; i6 < itemp; i6++) {
    b_A->data[i6] = A->data[i6];
  }

  b_m = b_A->size[0];
  if (b_A->size[0] <= 2) {
    b_mn = b_A->size[0];
  } else {
    b_mn = 2;
  }

  for (i6 = 0; i6 < 2; i6++) {
    jpvt[i6] = (signed char)(1 + i6);
  }

  if (b_A->size[0] == 0) {
  } else {
    for (i = 0; i < 2; i++) {
      work[i] = 0.0;
    }

    k = 1;
    for (pvt = 0; pvt < 2; pvt++) {
      smax = eml_xnrm2(b_m, b_A, k);
      vn2[pvt] = smax;
      k += b_m;
      vn1[pvt] = smax;
    }

    for (i = 0; i + 1 <= b_mn; i++) {
      i_i = i + i * b_m;
      mmi = (b_m - i) - 1;
      itemp = 0;
      if (2 - i > 1) {
        smax = fabs(vn1[i]);
        k = 2;
        while (k <= 2 - i) {
          temp2 = fabs(vn1[1]);
          if (temp2 > smax) {
            itemp = 1;
            smax = temp2;
          }

          k = 3;
        }
      }

      pvt = i + itemp;
      if (pvt + 1 != i + 1) {
        ix = b_m * pvt;
        iy = b_m * i;
        for (k = 1; k <= b_m; k++) {
          smax = b_A->data[ix];
          b_A->data[ix] = b_A->data[iy];
          b_A->data[iy] = smax;
          ix++;
          iy++;
        }

        itemp = jpvt[pvt];
        jpvt[pvt] = jpvt[i];
        jpvt[i] = (signed char)itemp;
        vn1[pvt] = vn1[i];
        vn2[pvt] = vn2[i];
      }

      if (i + 1 < b_m) {
        atmp = b_A->data[i_i];
        temp2 = 0.0;
        if (1 + mmi <= 0) {
        } else {
          smax = b_eml_xnrm2(mmi, b_A, i_i + 2);
          if (smax != 0.0) {
            smax = hypot(b_A->data[i_i], smax);
            if (b_A->data[i_i] >= 0.0) {
              smax = -smax;
            }

            if (fabs(smax) < 1.0020841800044864E-292) {
              pvt = 0;
              do {
                pvt++;
                eml_xscal(mmi, 9.9792015476736E+291, b_A, i_i + 2);
                smax *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(fabs(smax) >= 1.0020841800044864E-292));

              smax = b_eml_xnrm2(mmi, b_A, i_i + 2);
              smax = hypot(atmp, smax);
              if (atmp >= 0.0) {
                smax = -smax;
              }

              temp2 = (smax - atmp) / smax;
              eml_xscal(mmi, 1.0 / (atmp - smax), b_A, i_i + 2);
              for (k = 1; k <= pvt; k++) {
                smax *= 1.0020841800044864E-292;
              }

              atmp = smax;
            } else {
              temp2 = (smax - b_A->data[i_i]) / smax;
              eml_xscal(mmi, 1.0 / (b_A->data[i_i] - smax), b_A, i_i + 2);
              atmp = smax;
            }
          }
        }

        tau_data[i] = temp2;
      } else {
        atmp = b_A->data[i_i];
        tau_data[i] = eml_matlab_zlarfg();
      }

      b_A->data[i_i] = atmp;
      if (i + 1 < 2) {
        atmp = b_A->data[i_i];
        b_A->data[i_i] = 1.0;
        if (tau_data[0] != 0.0) {
          lastv = mmi + 1;
          itemp = i_i + mmi;
          while ((lastv > 0) && (b_A->data[itemp] == 0.0)) {
            lastv--;
            itemp--;
          }

          lastc = 1;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            itemp = b_m + 1;
            do {
              exitg1 = 0;
              if (itemp <= b_m + lastv) {
                if (b_A->data[itemp - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  itemp++;
                }
              } else {
                lastc = 0;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = 0;
          lastc = 0;
        }

        if (lastv > 0) {
          if (lastc == 0) {
          } else {
            work[0] = 0.0;
            iy = 0;
            pvt = 1 + b_m;
            while ((b_m > 0) && (pvt <= b_m + 1)) {
              ix = i_i;
              smax = 0.0;
              i6 = (pvt + lastv) - 1;
              for (itemp = pvt; itemp <= i6; itemp++) {
                smax += b_A->data[itemp - 1] * b_A->data[ix];
                ix++;
              }

              work[iy] += smax;
              iy++;
              pvt += b_m;
            }
          }

          if (-tau_data[0] == 0.0) {
          } else {
            k = b_m;
            iy = 0;
            pvt = 1;
            while (pvt <= lastc) {
              if (work[iy] != 0.0) {
                smax = work[iy] * -tau_data[0];
                ix = i_i;
                i6 = lastv + k;
                for (itemp = k; itemp + 1 <= i6; itemp++) {
                  b_A->data[itemp] += b_A->data[ix] * smax;
                  ix++;
                }
              }

              iy++;
              k += b_m;
              pvt = 2;
            }
          }
        }

        b_A->data[i_i] = atmp;
      }

      pvt = i + 2;
      while (pvt < 3) {
        itemp = (i + b_m) + 1;
        if (vn1[1] != 0.0) {
          smax = fabs(b_A->data[i + b_A->size[0]]) / vn1[1];
          smax = 1.0 - smax * smax;
          if (smax < 0.0) {
            smax = 0.0;
          }

          temp2 = vn1[1] / vn2[1];
          temp2 = smax * (temp2 * temp2);
          if (temp2 <= 1.4901161193847656E-8) {
            if (i + 1 < b_m) {
              smax = 0.0;
              if (mmi < 1) {
              } else if (mmi == 1) {
                smax = fabs(b_A->data[itemp]);
              } else {
                temp2 = 2.2250738585072014E-308;
                pvt = itemp + mmi;
                while (itemp + 1 <= pvt) {
                  atmp = fabs(b_A->data[itemp]);
                  if (atmp > temp2) {
                    t = temp2 / atmp;
                    smax = 1.0 + smax * t * t;
                    temp2 = atmp;
                  } else {
                    t = atmp / temp2;
                    smax += t * t;
                  }

                  itemp++;
                }

                smax = temp2 * sqrt(smax);
              }

              vn1[1] = smax;
              vn2[1] = smax;
            } else {
              vn1[1] = 0.0;
              vn2[1] = 0.0;
            }
          } else {
            vn1[1] *= sqrt(smax);
          }
        }

        pvt = 3;
      }
    }
  }

  *rankR = 0.0;
  if (mn + 1 > 0) {
    smax = fmax(A->size[0], 2.0) * fabs(b_A->data[0]) * 2.2204460492503131E-16;
    k = 0;
    while ((k <= mn) && (!(fabs(b_A->data[k + b_A->size[0] * k]) <= smax))) {
      (*rankR)++;
      k++;
    }
  }

  for (i = 0; i < 2; i++) {
    Y[i] = 0.0;
  }

  for (pvt = 0; pvt <= mn; pvt++) {
    if (tau_data[pvt] != 0.0) {
      smax = B->data[pvt];
      i6 = m - pvt;
      for (i = 0; i <= i6; i++) {
        b_i = ((unsigned int)pvt + i) + 2U;
        smax += b_A->data[((int)b_i + b_A->size[0] * pvt) - 1] * B->data[(int)
          b_i - 1];
      }

      smax *= tau_data[pvt];
      if (smax != 0.0) {
        B->data[pvt] -= smax;
        i6 = m - pvt;
        for (i = 0; i <= i6; i++) {
          b_i = ((unsigned int)pvt + i) + 2U;
          B->data[(int)b_i - 1] -= b_A->data[((int)b_i + b_A->size[0] * pvt) - 1]
            * smax;
        }
      }
    }
  }

  for (i = 0; i <= mn; i++) {
    Y[jpvt[i] - 1] = B->data[i];
  }

  for (pvt = 0; pvt <= mn; pvt++) {
    itemp = mn - pvt;
    Y[jpvt[itemp] - 1] /= b_A->data[itemp + b_A->size[0] * itemp];
    i = 0;
    while (i <= itemp - 1) {
      Y[jpvt[0] - 1] -= Y[jpvt[itemp] - 1] * b_A->data[b_A->size[0] * itemp];
      i = 1;
    }
  }

  emxFree_real_T(&b_A);
}

static double eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

static void eml_xscal(int n, double a, emxArray_real_T *x, int ix0)
{
  int i29;
  int k;
  i29 = (ix0 + n) - 1;
  for (k = ix0; k <= i29; k++) {
    x->data[k - 1] *= a;
  }
}

void b_polyfit(const emxArray_real_T *x, const emxArray_real_T *y, double p[3])
{
  emxArray_real_T *V;
  int n;
  unsigned int unnamed_idx_0;
  int i22;
  int k;
  emxArray_real_T *b_y;
  double rr;
  double p1[3];
  emxInit_real_T(&V, 2);
  n = x->size[0] - 1;
  unnamed_idx_0 = (unsigned int)x->size[0];
  i22 = V->size[0] * V->size[1];
  V->size[0] = (int)unnamed_idx_0;
  V->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)V, i22, (int)sizeof(double));
  if ((int)unnamed_idx_0 == 0) {
  } else {
    for (k = 0; k <= n; k++) {
      V->data[k + (V->size[0] << 1)] = 1.0;
    }

    for (k = 0; k <= n; k++) {
      V->data[k + V->size[0]] = x->data[k];
    }

    for (k = 0; k <= n; k++) {
      V->data[k] = x->data[k] * V->data[k + V->size[0]];
    }
  }

  b_emxInit_real_T(&b_y, 1);
  i22 = b_y->size[0];
  b_y->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)b_y, i22, (int)sizeof(double));
  k = y->size[0];
  for (i22 = 0; i22 < k; i22++) {
    b_y->data[i22] = y->data[i22];
  }

  b_eml_qrsolve(V, b_y, p1, &rr);
  emxFree_real_T(&b_y);
  emxFree_real_T(&V);
  for (i22 = 0; i22 < 3; i22++) {
    p[i22] = p1[i22];
  }
}

void polyfit(const emxArray_real_T *x, const emxArray_real_T *y, double p[2])
{
  emxArray_real_T *V;
  int n;
  unsigned int unnamed_idx_0;
  int i5;
  int k;
  emxArray_real_T *b_y;
  double rr;
  double p1[2];
  emxInit_real_T(&V, 2);
  n = x->size[0] - 1;
  unnamed_idx_0 = (unsigned int)x->size[0];
  i5 = V->size[0] * V->size[1];
  V->size[0] = (int)unnamed_idx_0;
  V->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)V, i5, (int)sizeof(double));
  if ((int)unnamed_idx_0 == 0) {
  } else {
    for (k = 0; k <= n; k++) {
      V->data[k + V->size[0]] = 1.0;
    }

    for (k = 0; k <= n; k++) {
      V->data[k] = x->data[k];
    }
  }

  b_emxInit_real_T(&b_y, 1);
  i5 = b_y->size[0];
  b_y->size[0] = y->size[0];
  emxEnsureCapacity((emxArray__common *)b_y, i5, (int)sizeof(double));
  k = y->size[0];
  for (i5 = 0; i5 < k; i5++) {
    b_y->data[i5] = y->data[i5];
  }

  eml_qrsolve(V, b_y, p1, &rr);
  emxFree_real_T(&b_y);
  emxFree_real_T(&V);
  for (i5 = 0; i5 < 2; i5++) {
    p[i5] = p1[i5];
  }
}

/* End of code generation (polyfit.c) */
