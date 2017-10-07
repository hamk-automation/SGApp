#include "libs.h"
#include "gl_canvas.h"
#include "frame.h"
#include "timer.h"
#include "utils.h"
#include "global_var.h"

// Create event table for the frame
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(ID_FILEOPEN, MyFrame::OnFileOpen)
  EVT_MENU(ID_SETVELOCITY, MyFrame::OnSetVelocity)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title)
{
  // Initializing GUI
  wxMenu* menu_file = new wxMenu();
  menu_file->Append(ID_FILEOPEN, "&Open file...\tCtrl-O", "Open a record file");
  menu_file->AppendSeparator();
  menu_file->Append(wxID_EXIT);

  wxMenu* menu_settings = new wxMenu();
  menu_settings->Append(ID_SETVELOCITY, "Velocity...", "Set the velocity");

  wxMenu* menu_help = new wxMenu();
  menu_help->Append(wxID_ABOUT);

  wxMenuBar* menu_bar = new wxMenuBar();
  menu_bar->Append(menu_file, "File");
  menu_bar->Append(menu_settings, "Settings");
  menu_bar->Append(menu_help, "Help");

  SetMenuBar(menu_bar);

  CreateStatusBar();

  SetClientSize(800, 600);
  char* st = new char[256];
  sprintf(st, "Velocity = %f", g_velocity_setpoint);
  SetStatusText(st);
  delete[] st;

  Show(true);

  m_canvas = new GLCanvas(this, GetClientSize()); // Create OpenGL window
}

MyFrame::~MyFrame()
{
  delete m_canvas;
}

void MyFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
  wxMessageBox("SGApp program - Developed by team SuomiGenius",
               "About SGApp", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnFileOpen(wxCommandEvent& event)
{
  wxFileDialog dialog(
                    this,
                    wxT("Open file"),
                    wxT("./in/"),
                    wxEmptyString,
                    wxT("Data files (*.dat)|*.dat")
                   );

  dialog.CentreOnParent();
  dialog.SetDirectory(wxGetHomeDir());

  if (dialog.ShowModal() == wxID_OK)
  {
    int n = strlen(dialog.GetPath().c_str());
    delete[] g_input_filename;
    g_input_filename = new char[n+1];
    strcpy(g_input_filename, dialog.GetPath().c_str());

    if (g_sg_app->ReloadRecordFile())
    {
      char* st = new char[256];
      sprintf(st, "SGApp - Visualization program [%s]", g_input_filename);
      SetLabel(wxString(st));
      delete[] st;
      m_canvas->RefreshGLCanvas();
    }
    else
    {
      delete[] g_input_filename;
      g_input_filename = NULL;
      SetLabel(wxT("SGApp - Visualization program"));
    }
  }
}

void MyFrame::OnSetVelocity(wxCommandEvent& event)
{
  int res = wxGetNumberFromUser(wxT("Velocity is set here"), wxT("Enter a number:"), wxT("Velocity"),
                                (int)g_velocity_setpoint, 0, 1000, this);
  if (res != -1 && res != 0)
  {
    g_velocity_setpoint = res;
    if (g_sg_app->ReloadRecordFile())
    {
      char* st = new char[256];
      sprintf(st, "Velocity = %f", g_velocity_setpoint);
      SetStatusText(st);
      delete[] st;
      m_canvas->RefreshGLCanvas();
    }
    else
    {
      delete[] g_input_filename;
      g_input_filename = NULL;
      SetLabel(wxT("SGApp - Visualization program"));
    }
  }
}
