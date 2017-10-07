#include "libs.h"
#include "utils.h"
#include "timer.h"

static Timer timer;

namespace Utils
{
  bool PrefixMatched(const char* prefix, const char* str)
  {
    int m = strlen(prefix);
    int n = strlen(str);
    if (m>n) {
      return false;
    }
    for (int i=0; i<m; ++i) {
      if (prefix[i] != str[i]) {
        return false;
      }
    }
    return true;
  }

  void Delay(const int64_t micros)
  {
    timer.Reset();
    timer.Wait(micros);
  }

  static emxArray_real_T* argInit_Unboundedx1_real_T(int data_size, double* data)
  {
    emxArray_real_T *result;
    int iv0[1]; iv0[0] = data_size;
    // Set the size of the array.
    // Change this size to the value that the application requires.
    result = emxCreateND_real_T(1, *(int (*)[1])&iv0[0]);
    // Loop over the array to initialize each element.
    for (int b_j0 = 0; b_j0 < result->size[0U]; b_j0++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result->data[b_j0] = data[b_j0];
    }
    return result;
  }

  static void ExtractFilterData(int data_size, double* data, const emxArray_real_T* source)
  {
    // Loop over the array to initialize each element.
    for (int b_j0 = 0; b_j0 < source->size[0U]; b_j0++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      data[b_j0] = source->data[b_j0];
    }
  }

  void FilterData(const uint32_t data_size, double* data_in, double** out_data)
  {
    emxArray_real_T *data_out;
    emxArray_real_T *M;
    emxInitArray_real_T(&data_out, 1);

    // Initialize function 'data_filter' input arguments.
    // Initialize function input argument 'M'.
    M = argInit_Unboundedx1_real_T(data_size, data_in);

    // Call the entry-point 'data_filter'.
    data_filter(M, data_out);

    ExtractFilterData(data_size, *out_data, data_out);

    emxDestroyArray_real_T(data_out);
    emxDestroyArray_real_T(M);
  }
}
