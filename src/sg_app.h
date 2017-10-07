#ifndef _SG_APP_H_
#define _SG_APP_H_

#include "libs.h"
#include "utils.h"

class SGApp
{
private:
  void FreeData();

public:
  SGApp();
  ~SGApp();

  bool Init();
  bool ReloadRecordFile();
  void UpdateData();
  void Render();
  void Zoom(float dz);
  void RotateCamera(const wxPoint& mouse_mic);
  void DragCamera(const wxPoint& mouse_mic);
  void CreateGLBufferObjects();
  void DeleteGLBufferObjects();
};

#endif // _SG_APP_H_
