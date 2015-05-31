/*
 * pwchcore.c
 *
 * Code generation for function 'pwchcore'
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
#include "pwchcore.h"
#include "bridge_ufb_emxutil.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void b_pwchcore(const emxArray_real_T *x, const emxArray_real_T *y, int yoffset,
                const emxArray_real_T *s, const emxArray_real_T *dx, const
                emxArray_real_T *divdif, emxArray_real_T *pp_breaks,
                emxArray_real_T *pp_coefs)
{
  int x_idx_0;
  int joffset;
  int nyrows;
  int cpage;
  int szc[3];
  double dxj;
  int i;
  double dzdxdx;
  double dzzdx;
  x_idx_0 = x->size[0];
  joffset = pp_breaks->size[0] * pp_breaks->size[1];
  pp_breaks->size[0] = 1;
  pp_breaks->size[1] = x_idx_0;
  emxEnsureCapacity((emxArray__common *)pp_breaks, joffset, (int)sizeof(double));
  for (joffset = 0; joffset < x_idx_0; joffset++) {
    pp_breaks->data[pp_breaks->size[0] * joffset] = x->data[joffset];
  }

  nyrows = s->size[0];
  cpage = s->size[0] * (x->size[0] - 1);
  for (joffset = 0; joffset < 2; joffset++) {
    szc[joffset] = s->size[joffset];
  }

  joffset = pp_coefs->size[0] * pp_coefs->size[1] * pp_coefs->size[2];
  pp_coefs->size[0] = szc[0];
  pp_coefs->size[1] = x->size[0] - 1;
  pp_coefs->size[2] = 4;
  emxEnsureCapacity((emxArray__common *)pp_coefs, joffset, (int)sizeof(double));
  for (x_idx_0 = 0; x_idx_0 + 1 < x->size[0]; x_idx_0++) {
    dxj = dx->data[x_idx_0];
    joffset = x_idx_0 * nyrows;
    for (i = 0; i + 1 <= nyrows; i++) {
      dzdxdx = divdif->data[joffset + i] - s->data[joffset + i];
      dzzdx = dzdxdx / dxj;
      dzdxdx = s->data[(joffset + nyrows) + i] - divdif->data[joffset + i];
      dzdxdx /= dxj;
      pp_coefs->data[joffset + i] = (dzdxdx - dzzdx) / dxj;
      pp_coefs->data[(cpage + joffset) + i] = 2.0 * dzzdx - dzdxdx;
      pp_coefs->data[((cpage << 1) + joffset) + i] = s->data[joffset + i];
      pp_coefs->data[(3 * cpage + joffset) + i] = y->data[(yoffset + joffset) +
        i];
    }
  }
}

void pwchcore(const emxArray_real_T *x, const emxArray_real_T *y, int yoffset,
              const emxArray_real_T *s, emxArray_real_T *pp_breaks,
              emxArray_real_T *pp_coefs)
{
  int x_idx_0;
  int joffset;
  int nyrows;
  int cpage;
  int szc[3];
  double dxj;
  int i;
  double dzdxdx;
  double divdifij;
  double dzzdx;
  x_idx_0 = x->size[0];
  joffset = pp_breaks->size[0] * pp_breaks->size[1];
  pp_breaks->size[0] = 1;
  pp_breaks->size[1] = x_idx_0;
  emxEnsureCapacity((emxArray__common *)pp_breaks, joffset, (int)sizeof(double));
  for (joffset = 0; joffset < x_idx_0; joffset++) {
    pp_breaks->data[pp_breaks->size[0] * joffset] = x->data[joffset];
  }

  nyrows = s->size[0];
  cpage = s->size[0] * (x->size[0] - 1);
  for (joffset = 0; joffset < 2; joffset++) {
    szc[joffset] = s->size[joffset];
  }

  joffset = pp_coefs->size[0] * pp_coefs->size[1] * pp_coefs->size[2];
  pp_coefs->size[0] = szc[0];
  pp_coefs->size[1] = x->size[0] - 1;
  pp_coefs->size[2] = 4;
  emxEnsureCapacity((emxArray__common *)pp_coefs, joffset, (int)sizeof(double));
  for (x_idx_0 = 1; x_idx_0 < x->size[0]; x_idx_0++) {
    dxj = x->data[x_idx_0] - x->data[x_idx_0 - 1];
    joffset = (x_idx_0 - 1) * nyrows;
    for (i = 0; i + 1 <= nyrows; i++) {
      dzdxdx = y->data[((yoffset + nyrows) + joffset) + i] - y->data[(yoffset +
        joffset) + i];
      divdifij = dzdxdx / dxj;
      dzdxdx = divdifij - s->data[joffset + i];
      dzzdx = dzdxdx / dxj;
      dzdxdx = s->data[(joffset + nyrows) + i] - divdifij;
      dzdxdx /= dxj;
      pp_coefs->data[joffset + i] = (dzdxdx - dzzdx) / dxj;
      pp_coefs->data[(cpage + joffset) + i] = 2.0 * dzzdx - dzdxdx;
      pp_coefs->data[((cpage << 1) + joffset) + i] = s->data[joffset + i];
      pp_coefs->data[(3 * cpage + joffset) + i] = y->data[(yoffset + joffset) +
        i];
    }
  }
}

/* End of code generation (pwchcore.c) */
