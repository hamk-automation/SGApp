#ifndef _LIBS_H_
#define _LIBS_H_

#define DEBUG

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <math.h>
#include <stdint.h>

#ifdef __linux__
    #define GL_GLEXT_PROTOTYPES
    #include <GL/glext.h>
    #include <wx/wxprec.h>
    #ifndef WX_PRECOMP
        #include <wx/wx.h>
        #include <wx/numdlg.h>
    #endif
    #include <wx/glcanvas.h>
#else // Windows
    #define UNICODE
    #include <windows.h>
    #include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/numdlg.h>
#endif
#include <wx/glcanvas.h>

#include "data_filter/rt_nonfinite.h"
#include "data_filter/data_filter.h"
#include "data_filter/data_filter_terminate.h"
#include "data_filter/data_filter_emxAPI.h"
#include "data_filter/data_filter_initialize.h"

#ifdef DEBUG
#define DEBUG_PRINT(...) \
            do { fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); } while (0)
#else
#endif

#define ERROR_PRINT(...) \
            do { fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); } while (0)

#define PI 3.14159265359

#endif // _LIBS_H_
