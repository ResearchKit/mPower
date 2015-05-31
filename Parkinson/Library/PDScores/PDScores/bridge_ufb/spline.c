/*
 * spline.c
 *
 * Code generation for function 'spline'
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
#include "spline.h"
#include "bridge_ufb_emxutil.h"
#include "pwchcore.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void spline(const emxArray_real_T *x, const emxArray_real_T *y, emxArray_real_T *
            output_breaks, emxArray_real_T *output_coefs)
{
  int nx;
  int pglen;
  boolean_T has_endslopes;
  emxArray_real_T *t0_breaks;
  emxArray_real_T *t0_coefs;
  int szdvdf[2];
  int pgm1;
  emxArray_real_T *dx;
  int pgm2;
  int j;
  unsigned int szc[3];
  emxArray_real_T *c;
  double d1;
  double A;
  emxArray_real_T *dvdf;
  emxArray_real_T *s;
  emxArray_real_T *md;
  double d31;
  double dnnm2;
  double dvdf1;
  double b_x[2];
  int szs[2];
  int yoffset;
  int k;
  int pg;
  nx = x->size[0] - 1;
  pglen = y->size[0];
  has_endslopes = (y->size[1] == x->size[0] + 2);
  emxInit_real_T(&t0_breaks, 2);
  c_emxInit_real_T(&t0_coefs, 3);
  if (x->size[0] == 2) {
    if (has_endslopes) {
      for (pgm1 = 0; pgm1 < 2; pgm1++) {
        szdvdf[pgm1] = y->size[pgm1];
      }

      emxInit_real_T(&dx, 2);
      pgm1 = dx->size[0] * dx->size[1];
      dx->size[0] = szdvdf[0];
      dx->size[1] = 2;
      emxEnsureCapacity((emxArray__common *)dx, pgm1, (int)sizeof(double));
      pgm2 = (y->size[1] - 1) * y->size[0];
      for (j = 0; j + 1 <= pglen; j++) {
        dx->data[j] = y->data[j];
        dx->data[pglen + j] = y->data[pgm2 + j];
      }

      pwchcore(x, y, y->size[0], dx, t0_breaks, t0_coefs);
      pgm1 = output_breaks->size[0] * output_breaks->size[1];
      output_breaks->size[0] = 1;
      output_breaks->size[1] = t0_breaks->size[1];
      emxEnsureCapacity((emxArray__common *)output_breaks, pgm1, (int)sizeof
                        (double));
      pgm2 = t0_breaks->size[0] * t0_breaks->size[1];
      emxFree_real_T(&dx);
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_breaks->data[pgm1] = t0_breaks->data[pgm1];
      }

      pgm1 = output_coefs->size[0] * output_coefs->size[1] * output_coefs->size
        [2];
      output_coefs->size[0] = t0_coefs->size[0];
      output_coefs->size[1] = t0_coefs->size[1];
      output_coefs->size[2] = 4;
      emxEnsureCapacity((emxArray__common *)output_coefs, pgm1, (int)sizeof
                        (double));
      pgm2 = t0_coefs->size[0] * t0_coefs->size[1] * t0_coefs->size[2];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_coefs->data[pgm1] = t0_coefs->data[pgm1];
      }
    } else {
      for (pgm1 = 0; pgm1 < 2; pgm1++) {
        szc[pgm1] = (unsigned int)y->size[pgm1];
      }

      c_emxInit_real_T(&c, 3);
      pgm1 = c->size[0] * c->size[1] * c->size[2];
      c->size[0] = (int)szc[0];
      c->size[1] = 1;
      c->size[2] = 2;
      emxEnsureCapacity((emxArray__common *)c, pgm1, (int)sizeof(double));
      d1 = x->data[1] - x->data[0];
      for (j = 0; j + 1 <= pglen; j++) {
        A = y->data[pglen + j] - y->data[j];
        c->data[j] = A / d1;
        c->data[pglen + j] = y->data[j];
      }

      emxInit_real_T(&dx, 2);
      pgm1 = dx->size[0] * dx->size[1];
      dx->size[0] = 1;
      dx->size[1] = 2;
      emxEnsureCapacity((emxArray__common *)dx, pgm1, (int)sizeof(double));
      for (pgm1 = 0; pgm1 < 2; pgm1++) {
        dx->data[dx->size[0] * pgm1] = x->data[pgm1];
      }

      pgm1 = output_breaks->size[0] * output_breaks->size[1];
      output_breaks->size[0] = 1;
      output_breaks->size[1] = dx->size[1];
      emxEnsureCapacity((emxArray__common *)output_breaks, pgm1, (int)sizeof
                        (double));
      pgm2 = dx->size[0] * dx->size[1];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_breaks->data[pgm1] = dx->data[pgm1];
      }

      emxFree_real_T(&dx);
      pgm1 = output_coefs->size[0] * output_coefs->size[1] * output_coefs->size
        [2];
      output_coefs->size[0] = c->size[0];
      output_coefs->size[1] = 1;
      output_coefs->size[2] = 2;
      emxEnsureCapacity((emxArray__common *)output_coefs, pgm1, (int)sizeof
                        (double));
      pgm2 = c->size[0] * c->size[1] * c->size[2];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_coefs->data[pgm1] = c->data[pgm1];
      }

      emxFree_real_T(&c);
    }
  } else {
    c_emxInit_real_T(&c, 3);
    b_emxInit_real_T(&dx, 1);
    emxInit_real_T(&dvdf, 2);
    emxInit_real_T(&s, 2);
    b_emxInit_real_T(&md, 1);
    if ((x->size[0] == 3) && (!has_endslopes)) {
      for (pgm1 = 0; pgm1 < 2; pgm1++) {
        szc[pgm1] = (unsigned int)y->size[pgm1];
      }

      pgm1 = c->size[0] * c->size[1] * c->size[2];
      c->size[0] = (int)szc[0];
      c->size[1] = 1;
      c->size[2] = 3;
      emxEnsureCapacity((emxArray__common *)c, pgm1, (int)sizeof(double));
      d31 = x->data[2] - x->data[0];
      d1 = x->data[1] - x->data[0];
      dnnm2 = x->data[2] - x->data[1];
      for (j = 0; j + 1 <= pglen; j++) {
        A = y->data[pglen + j] - y->data[j];
        dvdf1 = A / d1;
        A = y->data[(pglen << 1) + j] - y->data[pglen + j];
        c->data[j] = (A / dnnm2 - dvdf1) / d31;
        c->data[pglen + j] = dvdf1 - c->data[j] * d1;
        c->data[(pglen << 1) + j] = y->data[j];
      }

      b_x[0] = x->data[0];
      b_x[1] = x->data[2];
      pgm1 = output_breaks->size[0] * output_breaks->size[1];
      output_breaks->size[0] = 1;
      output_breaks->size[1] = 2;
      emxEnsureCapacity((emxArray__common *)output_breaks, pgm1, (int)sizeof
                        (double));
      for (pgm1 = 0; pgm1 < 2; pgm1++) {
        output_breaks->data[output_breaks->size[0] * pgm1] = b_x[pgm1];
      }

      pgm1 = output_coefs->size[0] * output_coefs->size[1] * output_coefs->size
        [2];
      output_coefs->size[0] = c->size[0];
      output_coefs->size[1] = 1;
      output_coefs->size[2] = 3;
      emxEnsureCapacity((emxArray__common *)output_coefs, pgm1, (int)sizeof
                        (double));
      pgm2 = c->size[0] * c->size[1] * c->size[2];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_coefs->data[pgm1] = c->data[pgm1];
      }
    } else {
      if (has_endslopes) {
        for (pgm1 = 0; pgm1 < 2; pgm1++) {
          szdvdf[pgm1] = y->size[pgm1];
        }

        szdvdf[1] = y->size[1] - 3;
        for (pgm1 = 0; pgm1 < 2; pgm1++) {
          szs[pgm1] = y->size[pgm1];
        }

        szs[1] = y->size[1] - 2;
        yoffset = y->size[0];
      } else {
        for (pgm1 = 0; pgm1 < 2; pgm1++) {
          szdvdf[pgm1] = y->size[pgm1];
        }

        szdvdf[1] = y->size[1] - 1;
        for (pgm1 = 0; pgm1 < 2; pgm1++) {
          szs[pgm1] = y->size[pgm1];
        }

        yoffset = 0;
      }

      if (1 > x->size[0] - 1) {
        pgm1 = 1;
      } else {
        pgm1 = x->size[0];
      }

      pgm2 = dx->size[0];
      dx->size[0] = pgm1 - 1;
      emxEnsureCapacity((emxArray__common *)dx, pgm2, (int)sizeof(double));
      pgm1 = dvdf->size[0] * dvdf->size[1];
      dvdf->size[0] = szdvdf[0];
      dvdf->size[1] = szdvdf[1];
      emxEnsureCapacity((emxArray__common *)dvdf, pgm1, (int)sizeof(double));
      pgm1 = s->size[0] * s->size[1];
      s->size[0] = szs[0];
      s->size[1] = szs[1];
      emxEnsureCapacity((emxArray__common *)s, pgm1, (int)sizeof(double));
      for (k = 0; k + 1 <= nx; k++) {
        dx->data[k] = x->data[k + 1] - x->data[k];
        pgm1 = k * pglen;
        pg = yoffset + pgm1;
        pgm2 = yoffset + (k + 1) * pglen;
        for (j = 0; j + 1 <= pglen; j++) {
          A = y->data[pgm2 + j] - y->data[pg + j];
          dvdf->data[pgm1 + j] = A / dx->data[k];
        }
      }

      for (k = 1; k + 1 <= nx; k++) {
        pg = k * pglen;
        pgm1 = (k - 1) * pglen;
        for (j = 0; j + 1 <= pglen; j++) {
          s->data[pg + j] = 3.0 * (dx->data[k] * dvdf->data[pgm1 + j] + dx->
            data[k - 1] * dvdf->data[pg + j]);
        }
      }

      if (has_endslopes) {
        d31 = 0.0;
        dnnm2 = 0.0;
        for (j = 0; j + 1 <= pglen; j++) {
          s->data[j] = dx->data[1] * y->data[j];
        }

        pgm2 = (x->size[0] + 1) * y->size[0];
        pgm1 = (x->size[0] - 1) * y->size[0];
        for (j = 0; j + 1 <= pglen; j++) {
          s->data[pgm1 + j] = dx->data[nx - 2] * y->data[pgm2 + j];
        }
      } else {
        d31 = x->data[2] - x->data[0];
        dnnm2 = x->data[x->size[0] - 1] - x->data[x->size[0] - 3];
        d1 = dx->data[0];
        for (j = 0; j + 1 <= pglen; j++) {
          A = (d1 + 2.0 * d31) * dx->data[1] * dvdf->data[j] + d1 * d1 *
            dvdf->data[pglen + j];
          s->data[j] = A / d31;
        }

        pg = (x->size[0] - 1) * y->size[0];
        pgm1 = (x->size[0] - 2) * y->size[0];
        pgm2 = (x->size[0] - 3) * y->size[0];
        d1 = dx->data[x->size[0] - 2];
        for (j = 0; j + 1 <= pglen; j++) {
          A = (d1 + 2.0 * dnnm2) * dx->data[x->size[0] - 3] * dvdf->data[pgm1 +
            j] + d1 * d1 * dvdf->data[pgm2 + j];
          s->data[pg + j] = A / dnnm2;
        }
      }

      pgm1 = md->size[0];
      md->size[0] = x->size[0];
      emxEnsureCapacity((emxArray__common *)md, pgm1, (int)sizeof(double));
      md->data[0] = dx->data[1];
      md->data[x->size[0] - 1] = dx->data[x->size[0] - 3];
      for (k = 1; k + 1 <= nx; k++) {
        md->data[k] = 2.0 * (dx->data[k] + dx->data[k - 1]);
      }

      d1 = dx->data[1] / md->data[0];
      md->data[1] -= d1 * d31;
      for (j = 0; j + 1 <= pglen; j++) {
        s->data[pglen + j] -= d1 * s->data[j];
      }

      for (k = 2; k + 1 <= nx; k++) {
        d1 = dx->data[k] / md->data[k - 1];
        md->data[k] -= d1 * dx->data[k - 2];
        pg = k * pglen;
        pgm1 = (k - 1) * pglen;
        for (j = 0; j + 1 <= pglen; j++) {
          s->data[pg + j] -= d1 * s->data[pgm1 + j];
        }
      }

      d1 = dnnm2 / md->data[x->size[0] - 2];
      md->data[x->size[0] - 1] -= d1 * dx->data[x->size[0] - 3];
      pg = (x->size[0] - 1) * y->size[0];
      pgm1 = (x->size[0] - 2) * y->size[0];
      for (j = 0; j + 1 <= pglen; j++) {
        s->data[pg + j] -= d1 * s->data[pgm1 + j];
      }

      for (j = 0; j + 1 <= pglen; j++) {
        A = s->data[pg + j];
        s->data[pg + j] = A / md->data[nx];
      }

      for (k = x->size[0] - 1; k > 1; k--) {
        pg = (k - 1) * pglen;
        pgm2 = k * pglen;
        for (j = 0; j + 1 <= pglen; j++) {
          A = s->data[pg + j] - dx->data[k - 2] * s->data[pgm2 + j];
          s->data[pg + j] = A / md->data[k - 1];
        }
      }

      for (j = 0; j + 1 <= pglen; j++) {
        A = s->data[j] - d31 * s->data[pglen + j];
        s->data[j] = A / md->data[0];
      }

      b_pwchcore(x, y, yoffset, s, dx, dvdf, t0_breaks, t0_coefs);
      pgm1 = output_breaks->size[0] * output_breaks->size[1];
      output_breaks->size[0] = 1;
      output_breaks->size[1] = t0_breaks->size[1];
      emxEnsureCapacity((emxArray__common *)output_breaks, pgm1, (int)sizeof
                        (double));
      pgm2 = t0_breaks->size[0] * t0_breaks->size[1];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_breaks->data[pgm1] = t0_breaks->data[pgm1];
      }

      pgm1 = output_coefs->size[0] * output_coefs->size[1] * output_coefs->size
        [2];
      output_coefs->size[0] = t0_coefs->size[0];
      output_coefs->size[1] = t0_coefs->size[1];
      output_coefs->size[2] = 4;
      emxEnsureCapacity((emxArray__common *)output_coefs, pgm1, (int)sizeof
                        (double));
      pgm2 = t0_coefs->size[0] * t0_coefs->size[1] * t0_coefs->size[2];
      for (pgm1 = 0; pgm1 < pgm2; pgm1++) {
        output_coefs->data[pgm1] = t0_coefs->data[pgm1];
      }
    }

    emxFree_real_T(&md);
    emxFree_real_T(&s);
    emxFree_real_T(&dvdf);
    emxFree_real_T(&dx);
    emxFree_real_T(&c);
  }

  emxFree_real_T(&t0_coefs);
  emxFree_real_T(&t0_breaks);
}

/* End of code generation (spline.c) */
