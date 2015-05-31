/*
 * _coder_bridge_ufb_api.c
 *
 * Code generation for function 'extractaudiophon'
 *
 */

/* Include files */
#include "_coder_bridge_ufb_api.h"

/* Function Declarations */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rawaudio,
  const char_T *identifier, emxArray_real_T *y);
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static const mxArray *emlrt_marshallOut(void);
static const mxArray *b_emlrt_marshallOut(const real_T u);
static const mxArray *c_emlrt_marshallOut(const emxArray_real_T *u);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *gait, const
  char_T *identifier, emxArray_real_T *y);
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static const mxArray *d_emlrt_marshallOut(const real_T u[7]);
static const mxArray *e_emlrt_marshallOut(const real_T u[3]);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tap, const
  char_T *identifier, emxArray_real_T *y);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static const mxArray *f_emlrt_marshallOut(const real_T u[2]);
static real_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *srate,
  const char_T *identifier);
static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *g_emlrt_marshallOut(const real_T u[13]);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *ftvec, const
  char_T *identifier, emxArray_real_T *y);
static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static const mxArray *h_emlrt_marshallOut(const emxArray_real_T *u);
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *plim,
  const char_T *identifier))[2];
static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[2];
static const mxArray *i_emlrt_marshallOut(const int32_T u_size[2]);
static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static real_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
static void q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static real_T (*r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2];
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void b_emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);

/* Function Definitions */
void bridge_ufb_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void bridge_ufb_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

void bridge_ufb_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  bridge_ufb_xil_terminate();
}

void extractaudiophon_api(const mxArray *prhs[1], const mxArray *plhs[2])
{
  emxArray_real_T *rawaudio;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  mxMalloc(0U);
  mxMalloc(0U);
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &rawaudio, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "rawaudio", rawaudio);

  /* Invoke the target function */
  extractaudiophon(rawaudio);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut();
  plhs[1] = emlrt_marshallOut();
  rawaudio->canFreeData = false;
  emxFree_real_T(&rawaudio);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *rawaudio,
  const char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  b_emlrt_marshallIn(sp, emlrtAlias(rawaudio), &thisId, y);
  emlrtDestroyArray(&rawaudio);
}

static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  m_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *emlrt_marshallOut(void)
{
  const mxArray *y;
  static const int32_T iv0[2] = { 0, 0 };

  const mxArray *m0;
  static const int32_T iv1[2] = { 0, 0 };

  y = NULL;
  m0 = emlrtCreateNumericArray(2, iv0, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m0, NULL);
  emlrtSetDimensions((mxArray *)m0, iv1, 2);
  emlrtAssign(&y, m0);
  return y;
}

void fastdfa_api(const mxArray *prhs[1], const mxArray *plhs[3])
{
  emxArray_real_T *x;
  emxArray_real_T *intervals;
  emxArray_real_T *flucts;
  real_T alpha;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &x, 1, true);
  emxInit_real_T(&st, &intervals, 1, true);
  emxInit_real_T(&st, &flucts, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x", x);

  /* Invoke the target function */
  fastdfa(x, &alpha, intervals, flucts);

  /* Marshall function outputs */
  plhs[0] = b_emlrt_marshallOut(alpha);
  plhs[1] = c_emlrt_marshallOut(intervals);
  plhs[2] = c_emlrt_marshallOut(flucts);
  flucts->canFreeData = false;
  emxFree_real_T(&flucts);
  intervals->canFreeData = false;
  emxFree_real_T(&intervals);
  x->canFreeData = false;
  emxFree_real_T(&x);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static const mxArray *b_emlrt_marshallOut(const real_T u)
{
  const mxArray *y;
  const mxArray *m1;
  y = NULL;
  m1 = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m1);
  return y;
}

static const mxArray *c_emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  static const int32_T iv2[1] = { 0 };

  const mxArray *m2;
  y = NULL;
  m2 = emlrtCreateNumericArray(1, iv2, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m2, (void *)u->data);
  emlrtSetDimensions((mxArray *)m2, u->size, 1);
  emlrtAssign(&y, m2);
  return y;
}

void features_bga_api(const mxArray *prhs[1], const mxArray *plhs[1])
{
  real_T (*ft)[7];
  emxArray_real_T *gait;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  ft = (real_T (*)[7])mxMalloc(sizeof(real_T [7]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  b_emxInit_real_T(&st, &gait, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "gait", gait);

  /* Invoke the target function */
  features_bga(gait, *ft);

  /* Marshall function outputs */
  plhs[0] = d_emlrt_marshallOut(*ft);
  gait->canFreeData = false;
  emxFree_real_T(&gait);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *gait, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  d_emlrt_marshallIn(sp, emlrtAlias(gait), &thisId, y);
  emlrtDestroyArray(&gait);
}

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  n_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *d_emlrt_marshallOut(const real_T u[7])
{
  const mxArray *y;
  static const int32_T iv3[2] = { 0, 0 };

  const mxArray *m3;
  static const int32_T iv4[2] = { 1, 7 };

  y = NULL;
  m3 = emlrtCreateNumericArray(2, iv3, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m3, (void *)u);
  emlrtSetDimensions((mxArray *)m3, iv4, 2);
  emlrtAssign(&y, m3);
  return y;
}

void features_bpa_api(const mxArray *prhs[1], const mxArray *plhs[1])
{
  real_T (*ft)[3];
  emxArray_real_T *post;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  ft = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  b_emxInit_real_T(&st, &post, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "post", post);

  /* Invoke the target function */
  features_bpa(post, *ft);

  /* Marshall function outputs */
  plhs[0] = e_emlrt_marshallOut(*ft);
  post->canFreeData = false;
  emxFree_real_T(&post);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static const mxArray *e_emlrt_marshallOut(const real_T u[3])
{
  const mxArray *y;
  static const int32_T iv5[2] = { 0, 0 };

  const mxArray *m4;
  static const int32_T iv6[2] = { 1, 3 };

  y = NULL;
  m4 = emlrtCreateNumericArray(2, iv5, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m4, (void *)u);
  emlrtSetDimensions((mxArray *)m4, iv6, 2);
  emlrtAssign(&y, m4);
  return y;
}

void features_bta_api(const mxArray *prhs[1], const mxArray *plhs[1])
{
  real_T (*ft)[2];
  emxArray_real_T *tap;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  ft = (real_T (*)[2])mxMalloc(sizeof(real_T [2]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  b_emxInit_real_T(&st, &tap, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "tap", tap);

  /* Invoke the target function */
  features_bta(tap, *ft);

  /* Marshall function outputs */
  plhs[0] = f_emlrt_marshallOut(*ft);
  tap->canFreeData = false;
  emxFree_real_T(&tap);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tap, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  f_emlrt_marshallIn(sp, emlrtAlias(tap), &thisId, y);
  emlrtDestroyArray(&tap);
}

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  o_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static const mxArray *f_emlrt_marshallOut(const real_T u[2])
{
  const mxArray *y;
  static const int32_T iv7[2] = { 0, 0 };

  const mxArray *m5;
  static const int32_T iv8[2] = { 1, 2 };

  y = NULL;
  m5 = emlrtCreateNumericArray(2, iv7, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m5, (void *)u);
  emlrtSetDimensions((mxArray *)m5, iv8, 2);
  emlrtAssign(&y, m5);
  return y;
}

void features_bvav2_api(const mxArray *prhs[2], const mxArray *plhs[1])
{
  real_T (*ft)[13];
  emxArray_real_T *audio;
  real_T srate;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  ft = (real_T (*)[13])mxMalloc(sizeof(real_T [13]));
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &audio, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "audio", audio);
  srate = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "srate");

  /* Invoke the target function */
  features_bvav2(audio, srate, *ft);

  /* Marshall function outputs */
  plhs[0] = g_emlrt_marshallOut(*ft);
  audio->canFreeData = false;
  emxFree_real_T(&audio);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static real_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *srate,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = h_emlrt_marshallIn(sp, emlrtAlias(srate), &thisId);
  emlrtDestroyArray(&srate);
  return y;
}

static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = p_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *g_emlrt_marshallOut(const real_T u[13])
{
  const mxArray *y;
  static const int32_T iv9[2] = { 0, 0 };

  const mxArray *m6;
  static const int32_T iv10[2] = { 1, 13 };

  y = NULL;
  m6 = emlrtCreateNumericArray(2, iv9, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m6, (void *)u);
  emlrtSetDimensions((mxArray *)m6, iv10, 2);
  emlrtAssign(&y, m6);
  return y;
}

void features_ufb_api(const mxArray *prhs[7], const mxArray *plhs[1])
{
  emxArray_real_T *ftvec;
  emxArray_real_T *wvec;
  emxArray_real_T *ilog;
  emxArray_real_T *ftmin;
  emxArray_real_T *ftmax;
  real_T fbmin;
  real_T fbmax;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  b_emxInit_real_T(&st, &ftvec, 2, true);
  emxInit_real_T(&st, &wvec, 1, true);
  b_emxInit_real_T(&st, &ilog, 2, true);
  b_emxInit_real_T(&st, &ftmin, 2, true);
  b_emxInit_real_T(&st, &ftmax, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, false, -1);
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, -1);
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, false, -1);

  /* Marshall function inputs */
  i_emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "ftvec", ftvec);
  emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "wvec", wvec);
  i_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "ilog", ilog);
  i_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "ftmin", ftmin);
  i_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "ftmax", ftmax);
  fbmin = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "fbmin");
  fbmax = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "fbmax");

  /* Invoke the target function */
  fbmin = features_ufb(ftvec, wvec, ilog, ftmin, ftmax, fbmin, fbmax);

  /* Marshall function outputs */
  plhs[0] = b_emlrt_marshallOut(fbmin);
  ftmax->canFreeData = false;
  emxFree_real_T(&ftmax);
  ftmin->canFreeData = false;
  emxFree_real_T(&ftmin);
  ilog->canFreeData = false;
  emxFree_real_T(&ilog);
  wvec->canFreeData = false;
  emxFree_real_T(&wvec);
  ftvec->canFreeData = false;
  emxFree_real_T(&ftvec);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *ftvec, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  j_emlrt_marshallIn(sp, emlrtAlias(ftvec), &thisId, y);
  emlrtDestroyArray(&ftvec);
}

static void j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  q_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

void lomb_api(const mxArray *prhs[4], const mxArray *plhs[3])
{
  emxArray_real_T *t;
  emxArray_real_T *h;
  emxArray_real_T *f;
  emxArray_real_T *P;
  emxArray_real_T *prob;
  real_T ofac;
  real_T hifac;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &t, 1, true);
  emxInit_real_T(&st, &h, 1, true);
  emxInit_real_T(&st, &f, 1, true);
  emxInit_real_T(&st, &P, 1, true);
  emxInit_real_T(&st, &prob, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "t", t);
  emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "h", h);
  ofac = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "ofac");
  hifac = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "hifac");

  /* Invoke the target function */
  lomb(t, h, ofac, hifac, f, P, prob);

  /* Marshall function outputs */
  plhs[0] = c_emlrt_marshallOut(f);
  plhs[1] = c_emlrt_marshallOut(P);
  plhs[2] = c_emlrt_marshallOut(prob);
  prob->canFreeData = false;
  emxFree_real_T(&prob);
  P->canFreeData = false;
  emxFree_real_T(&P);
  f->canFreeData = false;
  emxFree_real_T(&f);
  h->canFreeData = false;
  emxFree_real_T(&h);
  t->canFreeData = false;
  emxFree_real_T(&t);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

void mfcc_api(const mxArray *prhs[1], const mxArray *plhs[1])
{
  emxArray_real_T *samples;
  emxArray_real_T *cepstra;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &samples, 1, true);
  b_emxInit_real_T(&st, &cepstra, 2, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "samples", samples);

  /* Invoke the target function */
  mfcc(samples, cepstra);

  /* Marshall function outputs */
  plhs[0] = h_emlrt_marshallOut(cepstra);
  cepstra->canFreeData = false;
  emxFree_real_T(&cepstra);
  samples->canFreeData = false;
  emxFree_real_T(&samples);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static const mxArray *h_emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  static const int32_T iv11[2] = { 0, 0 };

  const mxArray *m7;
  y = NULL;
  m7 = emlrtCreateNumericArray(2, iv11, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m7, (void *)u->data);
  emlrtSetDimensions((mxArray *)m7, u->size, 2);
  emlrtAssign(&y, m7);
  return y;
}

void swipep_api(const mxArray *prhs[8], const mxArray *plhs[3])
{
  emxArray_real_T *x;
  emxArray_real_T *p;
  emxArray_real_T *t;
  emxArray_real_T *s;
  real_T fs;
  real_T (*plim)[2];
  real_T dt;
  real_T dlog2p;
  real_T dERBs;
  real_T woverlap;
  real_T sTHR;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &x, 1, true);
  emxInit_real_T(&st, &p, 1, true);
  emxInit_real_T(&st, &t, 1, true);
  emxInit_real_T(&st, &s, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x", x);
  fs = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "fs");
  plim = k_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "plim");
  dt = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "dt");
  dlog2p = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "dlog2p");
  dERBs = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "dERBs");
  woverlap = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "woverlap");
  sTHR = g_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "sTHR");

  /* Invoke the target function */
  swipep(x, fs, *plim, dt, dlog2p, dERBs, woverlap, sTHR, p, t, s);

  /* Marshall function outputs */
  plhs[0] = c_emlrt_marshallOut(p);
  plhs[1] = c_emlrt_marshallOut(t);
  plhs[2] = c_emlrt_marshallOut(s);
  s->canFreeData = false;
  emxFree_real_T(&s);
  t->canFreeData = false;
  emxFree_real_T(&t);
  p->canFreeData = false;
  emxFree_real_T(&p);
  x->canFreeData = false;
  emxFree_real_T(&x);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *plim,
  const char_T *identifier))[2]
{
  real_T (*y)[2];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = l_emlrt_marshallIn(sp, emlrtAlias(plim), &thisId);
  emlrtDestroyArray(&plim);
  return y;
}
  static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[2]
{
  real_T (*y)[2];
  y = r_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

void vadsplitphon_api(const mxArray *prhs[1], const mxArray *plhs[2])
{
  emxArray_real_T *x;
  int32_T i_size[2];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  mxMalloc(0U);
  mxMalloc(0U);
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &x, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x", x);

  /* Invoke the target function */
  vadsplitphon(x, i_size);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut();
  plhs[1] = i_emlrt_marshallOut(i_size);
  x->canFreeData = false;
  emxFree_real_T(&x);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

static const mxArray *i_emlrt_marshallOut(const int32_T u_size[2])
{
  const mxArray *y;
  static const int32_T iv12[2] = { 0, 0 };

  const mxArray *m8;
  y = NULL;
  m8 = emlrtCreateNumericArray(2, iv12, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m8, NULL);
  emlrtSetDimensions((mxArray *)m8, u_size, 2);
  emlrtAssign(&y, m8);
  return y;
}

static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv13[1];
  boolean_T bv0[1];
  int32_T iv14[1];
  iv13[0] = -1;
  bv0[0] = true;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 1U, iv13, bv0, iv14);
  ret->size[0] = iv14[0];
  ret->allocatedSize = ret->size[0];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv15[2];
  boolean_T bv1[2];
  int32_T i0;
  static const boolean_T bv2[2] = { true, false };

  int32_T iv16[2];
  for (i0 = 0; i0 < 2; i0++) {
    iv15[i0] = 5 * i0 - 1;
    bv1[i0] = bv2[i0];
  }

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv15, bv1, iv16);
  ret->size[0] = iv16[0];
  ret->size[1] = iv16[1];
  ret->allocatedSize = ret->size[0] * ret->size[1];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv17[2];
  boolean_T bv3[2];
  int32_T i1;
  static const boolean_T bv4[2] = { true, false };

  int32_T iv18[2];
  for (i1 = 0; i1 < 2; i1++) {
    iv17[i1] = (i1 << 2) - 1;
    bv3[i1] = bv4[i1];
  }

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv17, bv3, iv18);
  ret->size[0] = iv18[0];
  ret->size[1] = iv18[1];
  ret->allocatedSize = ret->size[0] * ret->size[1];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static real_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, 0);
  ret = *(real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static void q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv19[2];
  boolean_T bv5[2];
  int32_T i2;
  static const boolean_T bv6[2] = { false, true };

  int32_T iv20[2];
  for (i2 = 0; i2 < 2; i2++) {
    iv19[i2] = 1 + -2 * i2;
    bv5[i2] = bv6[i2];
  }

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv19, bv5, iv20);
  ret->size[0] = iv20[0];
  ret->size[1] = iv20[1];
  ret->allocatedSize = ret->size[0] * ret->size[1];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static real_T (*r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[2]
{
  real_T (*ret)[2];
  int32_T iv21[2];
  int32_T i3;
  for (i3 = 0; i3 < 2; i3++) {
    iv21[i3] = 1 + i3;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv21);
  ret = (real_T (*)[2])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
  static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (real_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((void *)(*pEmxArray)->data);
    }

    emlrtFreeMex((void *)(*pEmxArray)->size);
    emlrtFreeMex((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

static void b_emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/* End of code generation (_coder_bridge_ufb_api.c) */
