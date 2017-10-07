#ifndef _FRAME_H_
#define _FRAME_H_

#include "gl_canvas.h"

class MyFrame : public wxFrame
{
public:
  MyFrame(const wxString& title);
  ~MyFrame();

  void SetCanvas(GLCanvas* canvas) { m_canvas = canvas; }
  GLCanvas* GetCanvas() { return m_canvas; }

private:
  void OnFileOpen(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnResize(wxSizeEvent& event);
  void OnSetVelocity(wxCommandEvent& event);

  GLCanvas* m_canvas;

  wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_FILEOPEN = 1,
  ID_SETVELOCITY,
  ID_RESIZE
};

#endif // _FRAME_H_
