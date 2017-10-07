//
// File: main.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 24-Nov-2016 16:51:51
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "data_filter.h"
#include "main.h"
#include "data_filter_terminate.h"
#include "data_filter_emxAPI.h"
#include "data_filter_initialize.h"

// Function Declarations
static emxArray_real_T *argInit_Unboundedx1_real_T();
static double argInit_real_T();
static void main_data_filter();

// Function Definitions

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *argInit_Unboundedx1_real_T()
{
  emxArray_real_T *result;
  static int iv0[1] = { 2 };

  int b_j0;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(1, *(int (*)[1])&iv0[0]);

  // Loop over the array to initialize each element.
  for (b_j0 = 0; b_j0 < result->size[0U]; b_j0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[b_j0] = argInit_real_T();
  }

  return result;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_data_filter()
{
  emxArray_real_T *data_out;
  emxArray_real_T *M;
  emxInitArray_real_T(&data_out, 1);

  // Initialize function 'data_filter' input arguments.
  // Initialize function input argument 'M'.
  M = argInit_Unboundedx1_real_T();

  // Call the entry-point 'data_filter'.
  data_filter(M, data_out);
  emxDestroyArray_real_T(data_out);
  emxDestroyArray_real_T(M);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  data_filter_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_data_filter();

  // Terminate the application.
  // You do not need to do this more than one time.
  data_filter_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
