#include "libs.h"
#include "app.h"
#include "gl_canvas.h"
#include "utils.h"
#include "sg_app.h"
#include "global_var.h"

#define FRAME_UPDATE_INTERVAL 16 // milliseconds

static wxPoint last_mouse_pos;
extern "C"
{
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
void GLCanvasTimer::Notify()
{
//  m_canvas->Refresh();
}

wxBEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
  EVT_SIZE(GLCanvas::OnSize)
  EVT_PAINT(GLCanvas::OnPaint)
  EVT_MOUSEWHEEL(GLCanvas::OnWheel)
  EVT_MOTION(GLCanvas::OnDragging)
  EVT_MIDDLE_DOWN(GLCanvas::OnMiddleDown)
  EVT_MIDDLE_UP(GLCanvas::OnMiddleUp)
wxEND_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent, wxSize canvas_size)
  : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, canvas_size,
    wxFULL_REPAINT_ON_RESIZE),
    m_middle_down(false)
{
  // Explicitly create a new rendering context instance for this canvas.
  m_glRC = new wxGLContext(this);
  SetCurrent(*m_glRC);

  GLenum r = glewInit();
  if (r != GLEW_OK)
  {
    ERROR_PRINT("Failed to initialize GLEW!");
  }
  InitGL();
  ResetProjectionMode();

  m_canvas_timer = new GLCanvasTimer(this);
  m_canvas_timer->Start(FRAME_UPDATE_INTERVAL);

  last_mouse_pos.x = last_mouse_pos.y = -1;
}

GLCanvas::~GLCanvas()
{
  m_canvas_timer->Stop();

  g_sg_app->DeleteGLBufferObjects();

  DEBUG_PRINT("GLCanvas deleting");

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  delete m_glRC;
  delete m_canvas_timer;
}

void GLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  SetCurrent(*m_glRC);
  g_sg_app->Render();
  glFlush();
  SwapBuffers();
}

void GLCanvas::OnSize(wxSizeEvent& event)
{
  if (!IsShownOnScreen()) {
    return;
  }
  SetCurrent(*m_glRC);
  ResetProjectionMode();
  Refresh();
}

void GLCanvas::OnWheel(wxMouseEvent& event)
{
  if (!m_middle_down)
  {
    g_sg_app->Zoom(event.GetWheelRotation()/120);
    Refresh();
  }
}

void GLCanvas::OnDragging(wxMouseEvent& event)
{
  wxPoint new_pos = event.GetLogicalPosition(wxClientDC(this));
  if (event.Dragging())
  {
    if (m_middle_down)
    {
      g_sg_app->DragCamera(new_pos-last_mouse_pos);
    }
    else
    {
      if (last_mouse_pos.x == -1)
      {
        g_sg_app->RotateCamera(wxPoint(0, 0));
      }
      else
      {
        g_sg_app->RotateCamera(new_pos-last_mouse_pos);
      }
    }
    Refresh();
  }
  last_mouse_pos = new_pos;
}

void GLCanvas::OnMiddleDown(wxMouseEvent& WXUNUSED(event))
{
  m_middle_down = true;
}

void GLCanvas::OnMiddleUp(wxMouseEvent& WXUNUSED(event))
{
  m_middle_down = false;
}

void GLCanvas::InitGL()
{
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glEnable(GL_SMOOTH);
  glShadeModel(GL_SMOOTH);
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
  g_sg_app->CreateGLBufferObjects();
}

void GLCanvas::ResetProjectionMode()
{
  if (!IsShownOnScreen())
  return;

  SetCurrent(*m_glRC);

  int w, h;
  GetClientSize(&w, &h);

  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, (GLfloat)w/h, 1.0, 1000000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void GLCanvas::RefreshGLCanvas()
{
  g_sg_app->DeleteGLBufferObjects();
  g_sg_app->CreateGLBufferObjects();
  Refresh();
}
