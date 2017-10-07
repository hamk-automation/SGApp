//
// File: data_filter_emxutil.h
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 24-Nov-2016 16:51:51
//
#ifndef __DATA_FILTER_EMXUTIL_H__
#define __DATA_FILTER_EMXUTIL_H__

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "data_filter_types.h"

// Function Declarations
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for data_filter_emxutil.h
//
// [EOF]
//
