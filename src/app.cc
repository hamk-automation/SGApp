#include "libs.h"
#include "timer.h"
#include "utils.h"
#include "frame.h"
#include "app.h"
#include "sg_app.h"
#include "global_var.h"

SGApp* g_sg_app = NULL;

bool MyApp::OnInit()
{
  g_sg_app = new SGApp();

  if (!g_sg_app->Init())
  {
    return false;
  }

  if (!wxApp::OnInit())
  {
    return false;
  }

  MyFrame* frame = new MyFrame(wxT("SGApp - Visualization program"));
  frame->Show();

  return true;
}

int MyApp::OnExit()
{
  delete g_sg_app;

  return wxApp::OnExit();
}
