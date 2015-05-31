/*
 * features_bvav2.c
 *
 * Code generation for function 'features_bvav2'
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
#include "rdivide.h"
#include "abs.h"
#include "diff.h"
#include "sqrt.h"
#include "mean.h"
#include "power.h"
#include "iqr.h"
#include "bridge_ufb_rtwutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void features_bvav2(const emxArray_real_T *audio, double srate, double ft[13])
{
  int iy;
  double istart;
  double iend;
  int i2;
  emxArray_real_T *audiotrim;
  int vlen;
  int i1;
  emxArray_boolean_T *i;
  emxArray_real_T *f0;
  emxArray_real_T *f0t;
  emxArray_real_T *f0p;
  double T;
  emxArray_real_T *b_f0;
  emxArray_int32_T *r13;
  double f0m;
  double f0j;
  double f0jr;
  emxArray_real_T *abuf;
  emxArray_real_T *l2amp;
  emxArray_real_T *y;
  emxArray_real_T *A;
  emxArray_real_T *r14;
  emxArray_real_T *r15;
  double b_y;
  int iyLead;
  emxArray_real_T *cep;
  emxArray_real_T *r16;
  double ashr;
  double cepm[13];
  emxArray_real_T *vwork;
  int iv0[2];
  int b_i;
  emxArray_real_T *c_y;
  int dimSize;
  emxArray_real_T *b_y1;
  double tmp2;
  emxArray_real_T *d_y;
  double e_y[13];
  double f_y[13];

  /*  Computes basic phonation test features. */
  /*  Inputs: */
  /*   audio - mono floating-point, [-1,+1] normalized phonation */
  /*           signal */
  /*   srate - sample rate in Hz */
  /*  */
  /*  (CC BY-SA 3.0) Max Little, 2014 */
  /*  Output feature vector */
  for (iy = 0; iy < 13; iy++) {
    ft[iy] = rtNaN;
  }

  /*  Ignore zero-length inputs */
  if (audio->size[0] == 0) {
  } else {
    /*  Trim away start/end of phonation */
    istart = floor(srate);
    iend = ceil(10.0 * srate);
    if (istart > audio->size[0]) {
      istart = 1.0;
    }

    if (iend > audio->size[0]) {
      iend = audio->size[0];
    }

    if (istart > iend) {
      iy = 0;
      i2 = 0;
    } else {
      iy = (int)istart - 1;
      i2 = (int)iend;
    }

    b_emxInit_real_T(&audiotrim, 1);
    vlen = audiotrim->size[0];
    audiotrim->size[0] = i2 - iy;
    emxEnsureCapacity((emxArray__common *)audiotrim, vlen, (int)sizeof(double));
    i1 = i2 - iy;
    for (vlen = 0; vlen < i1; vlen++) {
      audiotrim->data[vlen] = audio->data[iy + vlen];
    }

    emxInit_boolean_T(&i, 1);
    b_emxInit_real_T(&f0, 1);
    b_emxInit_real_T(&f0t, 1);
    b_emxInit_real_T(&f0p, 1);
    T = (double)(i2 - iy) / srate;

    /*  F0 features */
    /*  Get F0 estimates */
    b_swipep(audiotrim, srate, f0, f0t, f0p);

    /*  Strip away all low-probability poor F0 estimates */
    vlen = i->size[0];
    i->size[0] = f0p->size[0];
    emxEnsureCapacity((emxArray__common *)i, vlen, (int)sizeof(boolean_T));
    i1 = f0p->size[0];
    for (vlen = 0; vlen < i1; vlen++) {
      i->data[vlen] = (f0p->data[vlen] >= 0.2);
    }

    emxFree_real_T(&f0p);
    b_emxInit_real_T(&b_f0, 1);
    emxInit_int32_T(&r13, 1);
    eml_li_find(i, r13);
    vlen = b_f0->size[0];
    b_f0->size[0] = r13->size[0];
    emxEnsureCapacity((emxArray__common *)b_f0, vlen, (int)sizeof(double));
    i1 = r13->size[0];
    emxFree_boolean_T(&i);
    for (vlen = 0; vlen < i1; vlen++) {
      b_f0->data[vlen] = f0->data[r13->data[vlen] - 1];
    }

    emxFree_int32_T(&r13);

    /*  Median F0 */
    if (b_f0->size[0] == 0) {
      f0m = rtNaN;
    } else {
      f0m = vectormedian(b_f0);
    }

    /*  Two jitter estimates */
    diff(b_f0, f0);
    b_abs(f0, f0t);
    rdivide(f0t, 0.02, f0);
    f0j = mean(f0);
    diff(b_f0, f0);
    b_abs(f0, f0t);
    rdivide(f0t, 0.02, f0);
    emxFree_real_T(&f0t);
    emxFree_real_T(&b_f0);
    if (f0->size[0] == 0) {
      f0jr = rtNaN;
    } else {
      f0jr = vectormedian(f0);
    }

    emxInit_real_T(&abuf, 2);

    /*  Amplitude features */
    istart = rt_roundd_snf(0.05 * srate);

    /* abuf = buffer(audiotrim,ampwin); */
    iend = (((double)(i2 - iy) + istart) - 1.0) / istart;
    vlen = abuf->size[0] * abuf->size[1];
    abuf->size[0] = (int)istart;
    abuf->size[1] = (int)iend;
    emxEnsureCapacity((emxArray__common *)abuf, vlen, (int)sizeof(double));
    i1 = (int)istart * (int)iend;
    for (vlen = 0; vlen < i1; vlen++) {
      abuf->data[vlen] = 0.0;
    }

    vlen = f0->size[0];
    f0->size[0] = audiotrim->size[0];
    emxEnsureCapacity((emxArray__common *)f0, vlen, (int)sizeof(double));
    i1 = audiotrim->size[0];
    for (vlen = 0; vlen < i1; vlen++) {
      f0->data[vlen] = audiotrim->data[vlen];
    }

    emxInit_real_T(&l2amp, 2);
    emxInit_real_T(&y, 2);
    emxInit_real_T(&A, 2);
    emxInit_real_T(&r14, 2);
    emxInit_real_T(&r15, 2);
    buffer_nooverlap(&abuf->data[0], &f0->data[0], (double)(i2 - iy), istart);
    c_power(abuf, r14);
    b_mean(r14, l2amp);
    d_sqrt(l2amp);

    /*  Two shimmer estimates */
    b_diff(l2amp, r15);
    e_abs(r15, A);
    c_rdivide(A, 0.05, y);
    emxFree_real_T(&r14);
    emxFree_real_T(&f0);
    emxFree_real_T(&abuf);
    if (y->size[1] == 0) {
      b_y = 0.0;
    } else {
      b_y = y->data[0];
      for (iyLead = 2; iyLead <= y->size[1]; iyLead++) {
        b_y += y->data[iyLead - 1];
      }
    }

    emxInit_real_T(&cep, 2);
    emxInit_real_T(&r16, 2);
    b_diff(l2amp, r15);
    e_abs(r15, A);
    c_rdivide(A, 0.05, r15);
    ashr = median(r15);

    /*  Compute MFCCs */
    mfcc(audiotrim, r16);
    iy = cep->size[0] * cep->size[1];
    cep->size[0] = r16->size[1];
    cep->size[1] = 13;
    emxEnsureCapacity((emxArray__common *)cep, iy, (int)sizeof(double));
    emxFree_real_T(&r15);
    emxFree_real_T(&A);
    emxFree_real_T(&l2amp);
    emxFree_real_T(&audiotrim);
    for (iy = 0; iy < 13; iy++) {
      i1 = r16->size[1];
      for (i2 = 0; i2 < i1; i2++) {
        cep->data[i2 + cep->size[0] * iy] = r16->data[iy + r16->size[0] * i2];
      }
    }

    emxFree_real_T(&r16);

    /*  MFCC summaries across time */
    b_emxInit_real_T(&vwork, 1);
    if (cep->size[0] == 0) {
      for (iy = 0; iy < 13; iy++) {
        cepm[iy] = rtNaN;
      }
    } else {
      vlen = cep->size[0];
      iv0[0] = cep->size[0];
      iy = vwork->size[0];
      vwork->size[0] = iv0[0];
      emxEnsureCapacity((emxArray__common *)vwork, iy, (int)sizeof(double));
      i2 = 0;
      iy = -1;
      for (b_i = 0; b_i < 13; b_i++) {
        i1 = i2;
        i2 += vlen;
        for (iyLead = 0; iyLead < vlen; iyLead++) {
          vwork->data[iyLead] = cep->data[i1];
          i1++;
        }

        iy++;
        cepm[iy] = vectormedian(vwork);
      }
    }

    emxInit_real_T(&c_y, 2);
    iv0[1] = cep->size[0];
    iy = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 1;
    c_y->size[1] = iv0[1];
    emxEnsureCapacity((emxArray__common *)c_y, iy, (int)sizeof(double));
    if (iv0[1] >= 1) {
      c_y->data[iv0[1] - 1] = T;
      if (c_y->size[1] >= 2) {
        c_y->data[0] = 0.0;
        if (c_y->size[1] >= 3) {
          if ((T < 0.0) && (fabs(T) > 8.9884656743115785E+307)) {
            istart = T / ((double)c_y->size[1] - 1.0);
            iy = c_y->size[1];
            for (iyLead = 0; iyLead <= iy - 3; iyLead++) {
              c_y->data[iyLead + 1] = istart * (1.0 + (double)iyLead);
            }
          } else {
            istart = T / ((double)c_y->size[1] - 1.0);
            iy = c_y->size[1];
            for (iyLead = 0; iyLead <= iy - 3; iyLead++) {
              c_y->data[iyLead + 1] = (1.0 + (double)iyLead) * istart;
            }
          }
        }
      }
    }

    iy = vwork->size[0];
    vwork->size[0] = c_y->size[1];
    emxEnsureCapacity((emxArray__common *)vwork, iy, (int)sizeof(double));
    i1 = c_y->size[1];
    for (iy = 0; iy < i1; iy++) {
      vwork->data[iy] = c_y->data[c_y->size[0] * iy];
    }

    emxFree_real_T(&c_y);
    istart = vwork->data[1] - vwork->data[0];

    /*  MFCC jitter measure */
    dimSize = cep->size[0];
    emxFree_real_T(&vwork);
    emxInit_real_T(&b_y1, 2);
    if (cep->size[0] == 0) {
      iy = b_y1->size[0] * b_y1->size[1];
      b_y1->size[0] = 0;
      b_y1->size[1] = 13;
      emxEnsureCapacity((emxArray__common *)b_y1, iy, (int)sizeof(double));
    } else {
      if (cep->size[0] - 1 <= 1) {
        i2 = cep->size[0] - 1;
      } else {
        i2 = 1;
      }

      if (i2 < 1) {
        iy = b_y1->size[0] * b_y1->size[1];
        b_y1->size[0] = 0;
        b_y1->size[1] = 13;
        emxEnsureCapacity((emxArray__common *)b_y1, iy, (int)sizeof(double));
      } else {
        iy = b_y1->size[0] * b_y1->size[1];
        b_y1->size[0] = cep->size[0] - 1;
        b_y1->size[1] = 13;
        emxEnsureCapacity((emxArray__common *)b_y1, iy, (int)sizeof(double));
        i2 = cep->size[0] - 1;
        if (!(i2 == 0)) {
          i2 = 0;
          i1 = 0;
          for (vlen = 0; vlen < 13; vlen++) {
            iy = i2 + 1;
            iyLead = i1;
            iend = cep->data[i2];
            for (b_i = 2; b_i <= dimSize; b_i++) {
              T = cep->data[iy];
              tmp2 = iend;
              iend = T;
              T -= tmp2;
              iy++;
              b_y1->data[iyLead] = T;
              iyLead++;
            }

            i2 += dimSize;
            i1 = (i1 + dimSize) - 1;
          }
        }
      }
    }

    emxFree_real_T(&cep);
    for (iy = 0; iy < 2; iy++) {
      iv0[iy] = b_y1->size[iy];
    }

    emxInit_real_T(&d_y, 2);
    iy = d_y->size[0] * d_y->size[1];
    d_y->size[0] = iv0[0];
    d_y->size[1] = 13;
    emxEnsureCapacity((emxArray__common *)d_y, iy, (int)sizeof(double));
    iy = b_y1->size[0] * 13;
    for (iyLead = 0; iyLead < iy; iyLead++) {
      d_y->data[iyLead] = fabs(b_y1->data[iyLead]);
    }

    emxFree_real_T(&b_y1);
    iy = d_y->size[0] * d_y->size[1];
    d_y->size[1] = 13;
    emxEnsureCapacity((emxArray__common *)d_y, iy, (int)sizeof(double));
    vlen = d_y->size[0];
    i2 = d_y->size[1];
    i1 = vlen * i2;
    for (iy = 0; iy < i1; iy++) {
      d_y->data[iy] /= istart;
    }

    if (d_y->size[0] == 0) {
      memset(&e_y[0], 0, 13U * sizeof(double));
    } else {
      vlen = 0;
      iy = -1;
      for (b_i = 0; b_i < 13; b_i++) {
        i2 = vlen;
        vlen++;
        istart = d_y->data[i2];
        for (iyLead = 2; iyLead <= d_y->size[0]; iyLead++) {
          vlen++;
          istart += d_y->data[vlen - 1];
        }

        iy++;
        e_y[iy] = istart;
      }
    }

    /*  Output phonation feature vector */
    vlen = d_y->size[0];
    emxFree_real_T(&d_y);
    for (iy = 0; iy < 13; iy++) {
      f_y[iy] = e_y[iy] / (double)vlen;
    }

    ft[0] = f0m;
    ft[1] = f0j;
    ft[2] = f0jr;
    ft[3] = b_y / (double)y->size[1];
    ft[4] = ashr;
    emxFree_real_T(&y);
    for (iy = 0; iy < 4; iy++) {
      ft[iy + 5] = cepm[iy];
    }

    for (iy = 0; iy < 4; iy++) {
      ft[iy + 9] = f_y[iy];
    }
  }
}

/* End of code generation (features_bvav2.c) */
