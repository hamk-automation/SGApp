//
// File: data_filter_emxAPI.h
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 24-Nov-2016 16:51:51
//
#ifndef __DATA_FILTER_EMXAPI_H__
#define __DATA_FILTER_EMXAPI_H__

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "data_filter_types.h"

// Function Declarations
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for data_filter_emxAPI.h
//
// [EOF]
//
