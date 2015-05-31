/*
 * _coder_bridge_ufb_api.h
 *
 * Code generation for function 'extractaudiophon'
 *
 */

#ifndef ___CODER_BRIDGE_UFB_API_H__
#define ___CODER_BRIDGE_UFB_API_H__
/* Include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T
{
    real_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
};
#endif /*struct_emxArray_real_T*/
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /*typedef_emxArray_real_T*/

/* Function Declarations */
extern void bridge_ufb_initialize(emlrtContext *aContext);
extern void bridge_ufb_terminate(void);
extern void bridge_ufb_atexit(void);
extern void extractaudiophon_api(const mxArray *prhs[1], const mxArray *plhs[2]);
extern void extractaudiophon(emxArray_real_T *rawaudio);
extern void fastdfa_api(const mxArray *prhs[1], const mxArray *plhs[3]);
extern void fastdfa(emxArray_real_T *x, real_T *alpha, emxArray_real_T *intervals, emxArray_real_T *flucts);
extern void features_bga_api(const mxArray *prhs[1], const mxArray *plhs[1]);
extern void features_bga(emxArray_real_T *gait, real_T ft[7]);
extern void features_bpa_api(const mxArray *prhs[1], const mxArray *plhs[1]);
extern void features_bpa(emxArray_real_T *post, real_T ft[3]);
extern void features_bta_api(const mxArray *prhs[1], const mxArray *plhs[1]);
extern void features_bta(emxArray_real_T *tap, real_T ft[2]);
extern void features_bvav2_api(const mxArray *prhs[2], const mxArray *plhs[1]);
extern void features_bvav2(emxArray_real_T *audio, real_T srate, real_T ft[13]);
extern void features_ufb_api(const mxArray *prhs[7], const mxArray *plhs[1]);
extern real_T features_ufb(emxArray_real_T *ftvec, emxArray_real_T *wvec, emxArray_real_T *ilog, emxArray_real_T *ftmin, emxArray_real_T *ftmax, real_T fbmin, real_T fbmax);
extern void lomb_api(const mxArray *prhs[4], const mxArray *plhs[3]);
extern void lomb(emxArray_real_T *t, emxArray_real_T *h, real_T ofac, real_T hifac, emxArray_real_T *f, emxArray_real_T *P, emxArray_real_T *prob);
extern void mfcc_api(const mxArray *prhs[1], const mxArray *plhs[1]);
extern void mfcc(emxArray_real_T *samples, emxArray_real_T *cepstra);
extern void swipep_api(const mxArray *prhs[8], const mxArray *plhs[3]);
extern void swipep(emxArray_real_T *x, real_T fs, real_T plim[2], real_T dt, real_T dlog2p, real_T dERBs, real_T woverlap, real_T sTHR, emxArray_real_T *p, emxArray_real_T *t, emxArray_real_T *s);
extern void vadsplitphon_api(const mxArray *prhs[1], const mxArray *plhs[2]);
extern void vadsplitphon(emxArray_real_T *x, int32_T i_size[2]);
extern void bridge_ufb_xil_terminate(void);

#endif
/* End of code generation (_coder_bridge_ufb_api.h) */
