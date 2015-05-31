/*
 * bridge_ufb_terminate.c
 *
 * Code generation for function 'bridge_ufb_terminate'
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
#include "bridge_ufb_terminate.h"
#include "bridge_ufb_emxutil.h"
#include "bridge_ufb_data.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void bridge_ufb_terminate(void)
{
  emxFree_real_T(&primetable);
}

/* End of code generation (bridge_ufb_terminate.c) */
