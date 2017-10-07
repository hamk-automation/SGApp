#ifndef _GLOBAL_VAR_H_
#define _GLOBAL_VAR_H_

#include "libs.h"
#include "sg_app.h"
#include "road_record.h"

extern RoadRecord* g_road_rec;
extern SGApp* g_sg_app;
extern char* g_input_filename;
extern float g_velocity_setpoint;

#endif // _GLOBAL_VAR_H_
