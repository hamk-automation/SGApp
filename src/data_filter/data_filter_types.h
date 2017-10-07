//
// File: data_filter_types.h
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 24-Nov-2016 16:51:51
//
#ifndef __DATA_FILTER_TYPES_H__
#define __DATA_FILTER_TYPES_H__

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T
#endif

//
// File trailer for data_filter_types.h
//
// [EOF]
//
