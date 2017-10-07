#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

#include "libs.h"
#include "timer.h"

class GLCanvasTimer : public wxTimer
{
private:
  wxGLCanvas* m_canvas;

public:
  GLCanvasTimer(wxGLCanvas* target_canvas) { m_canvas = target_canvas; }
  void Notify();
};

class GLCanvas : public wxGLCanvas
{
private:
  wxGLContext* m_glRC; // OpenGL Rendering Context
  GLCanvasTimer* m_canvas_timer;
  bool m_middle_down;

  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnWheel(wxMouseEvent& event);
  void OnDragging(wxMouseEvent& event);
  void OnMiddleDown(wxMouseEvent& event);
  void OnMiddleUp(wxMouseEvent& event);

  void InitGL();
  void ResetProjectionMode();

public:
  GLCanvas(wxWindow* parent, wxSize canvas_size);
  virtual ~GLCanvas();

  void RefreshGLCanvas();

  wxDECLARE_EVENT_TABLE();
};

#endif // _GL_CANVAS_H_
