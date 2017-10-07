#ifndef _APP_H_
#define _APP_H_

#include "libs.h"

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
};

wxDECLARE_APP(MyApp);

#endif // _APP_H_
