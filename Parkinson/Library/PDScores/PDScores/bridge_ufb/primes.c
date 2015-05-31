/*
 * primes.c
 *
 * Code generation for function 'primes'
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
#include "primes.h"

/* Custom Source Code */
#include "fastdfa_core_nomex.h"
#include "buffer.h"
#include "signalprocessing.h"

/* Function Definitions */
void primes(double p_data[], int p_size[2])
{
  boolean_T isp[5000];
  int m;
  unsigned int nleft;
  int k;
  for (m = 0; m < 5000; m++) {
    isp[m] = true;
  }

  nleft = 5000U;
  for (k = 0; k < 98; k += 2) {
    if (isp[(int)(((k + 3) + 1U) >> 1) - 1]) {
      for (m = (int)(((unsigned int)(k + 3) * (k + 3) + 1U) >> 1); (unsigned int)
           m <= 5000U; m = (int)(((unsigned int)m + k) + 3U)) {
        if (isp[m - 1]) {
          isp[m - 1] = false;
          nleft--;
        }
      }
    }
  }

  p_size[0] = 1;
  p_size[1] = (int)nleft;
  m = 1;
  p_data[0] = 2.0;
  for (k = 0; k < 4999; k++) {
    if (isp[k + 1] && ((unsigned int)m < 1229U)) {
      m++;
      p_data[m - 1] = ((double)k + 2.0) * 2.0 - 1.0;
    }
  }
}

/* End of code generation (primes.c) */
