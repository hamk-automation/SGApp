/*
 * File: _coder_data_filter_api.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 24-Nov-2016 16:51:51
 */

#ifndef ___CODER_DATA_FILTER_API_H__
#define ___CODER_DATA_FILTER_API_H__

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_data_filter_api.h"

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

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void data_filter(emxArray_real_T *M, emxArray_real_T *data_out);
extern void data_filter_api(const mxArray *prhs[1], const mxArray *plhs[1]);
extern void data_filter_atexit(void);
extern void data_filter_initialize(void);
extern void data_filter_terminate(void);
extern void data_filter_xil_terminate(void);

#endif

/*
 * File trailer for _coder_data_filter_api.h
 *
 * [EOF]
 */
