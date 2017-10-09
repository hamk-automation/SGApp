#include "libs.h"
#include "sg_app.h"
#include "road_record.h"
#include "utils.h"
#include "matrix.h"

#define POINT_RADIUS            10.0f
#define BORDER_RADIUS           10000.0f
#define GRID_SIZE               100.0f
#define CLOSE_RANGE_LIMIT       60
#define FAR_RANGE_LIMIT         3000
#define X_AXIS_LIMIT            6*GRID_SIZE

// Global variables
RoadRecord* g_road_rec = NULL;
char* g_input_filename = NULL;
float g_velocity_setpoint = 100.0f;

// Local variables
static Utils::Vector3D camera_pos;
static Utils::Vector3D camera_at;

// static ScanDataPack* data_pack = NULL;
static Utils::Vector3D** points = NULL;
static Utils::ColorRGB** colors = NULL;
static bool** out_of_range = NULL;
static Utils::Vector3D*  points_gl_array;
static Utils::ColorRGB*  colors_gl_array;
static uint32_t numGlPoints;
static GLuint vbo = 0; // Vertex buffer object
static GLuint cbo = 0; // Color buffer object

// Local functions
static Utils::ColorRGB MakeColor(float vmin, float vmax, float v);
static void DrawGrids (void);
static void DrawCameraTarGet();

SGApp::SGApp()
{
}

SGApp::~SGApp()
{
  if (g_road_rec)
  {
    FreeData();
  }
  data_filter_terminate();
  delete[] g_input_filename;
}

void SGApp::FreeData()
{
  uint32_t n = g_road_rec->GetNumPacks();
  delete g_road_rec;
  g_road_rec = NULL;

  for (uint32_t i=0; i<n; ++i)
  {
    delete[] points[i];
    delete[] colors[i];
    delete[] out_of_range[i];
  }
  delete[] points;
  delete[] colors;
  delete[] out_of_range;
  delete[] points_gl_array;
  delete[] colors_gl_array;
}

bool SGApp::Init()
{
  data_filter_initialize();
  camera_pos.y = 3000.0;
  camera_pos.z = 5000.0;
  camera_at = Utils::Vector3D(0, 0, 0);
  return true;
}

bool SGApp::ReloadRecordFile()
{
  if (g_road_rec)
  {
    FreeData();
  }

  // Load new record file
  g_road_rec = new RoadRecord();
  if (!g_road_rec->ReadFromFile(g_input_filename))
  {
    ERROR_PRINT("Could not load %s!", g_input_filename);
    delete g_road_rec;
    return false;
  }

  g_road_rec->Filter();

  uint32_t n = g_road_rec->GetNumPacks();
  points = new Utils::Vector3D*[n];
  colors = new Utils::ColorRGB*[n];
  out_of_range = new bool*[g_road_rec->GetNumPacks()];

  for (uint32_t i=0; i<n; ++i)
  {
    float displacement = g_velocity_setpoint/15;
    double h0 = 251;
    uint32_t m = g_road_rec->GetData()[i].GetNumPoints();
    YawPitchRoll rotate_angles = g_road_rec->GetData()[i].GetRotationAngles();
    uint16_t* ranges = g_road_rec->GetData()[i].GetRangeList();
    float start_angle = g_road_rec->GetStartAngle();
    float angular_step = g_road_rec->GetAngularStep();

    points[i] = new Utils::Vector3D[m];
    colors[i] = new Utils::ColorRGB[m];
    out_of_range[i] = new bool[m];

    #define R 308.87
    #define ALPHA0 54.35
    #define RX 181.9349

    #define DX_SICK_LEFT_REAR RX
    #define DY_SICK_REAR 400
    #define DZ_SICK_REAR (251-90)

    #define DX_SICK_LEFT_FRONT 0
    #define DY_SICK_FRONT 250
    #define DZ_SICK_FRONT (251-90)

    // Initialize matrices
    Matrix sick_transform;
    sick_transform.LoadIdentity();
    sick_transform.RotateX(90);

    static bool pitch_flag = false;
    static bool pitching = false;
    if (rotate_angles.pitch < -3 || rotate_angles.pitch > 3)
    {
      if (!pitching)
      {
        pitch_flag = !pitch_flag;
      }
      pitching = true;
    }
    else if (rotate_angles.pitch>-1 && rotate_angles.pitch<1)
    {
      pitching = false;
    }

    // Using rear wheel for pitching
    Utils::Vector3D rotation_origin;
    if (rotate_angles.roll < 0)
    {
      rotation_origin.x = RX;
    }
    else
    {
      rotation_origin.x = -RX;
    }
    rotation_origin.y = DY_SICK_REAR;
    if (!pitch_flag)
    {
      rotation_origin.y = DY_SICK_FRONT;
    }
    rotation_origin.z = h0;

    sick_transform.Translate(rotation_origin);
    sick_transform.RotateY(rotate_angles.roll);
    sick_transform.RotateX(rotate_angles.pitch);
    Utils::Vector3D zero_vec(0, displacement * i, h0);
    sick_transform.Translate(zero_vec - rotation_origin);

    for (uint32_t j=0; j<m; ++j)
    {
      Matrix mat;
      mat.LoadIdentity();
      mat.Translate(
        Utils::Vector3D(
          (double)ranges[j] * COSD(start_angle + angular_step * j),
          (double)ranges[j] * SIND(start_angle + angular_step * j),
          0
        )
     );
      mat.Multiply(sick_transform);
      points[i][j] = mat.GetPosition();
      out_of_range[i][j] =
        g_road_rec->GetData()[i].GetRangeList()[j] <= CLOSE_RANGE_LIMIT ||
        g_road_rec->GetData()[i].GetRangeList()[j] >= FAR_RANGE_LIMIT ||
        (points[i][j].x > X_AXIS_LIMIT || points[i][j].x < -X_AXIS_LIMIT);

      colors[i][j] = MakeColor(-50, 50, points[i][j].z);
    }

    // Bounding the range list
    int j = 0;
    while (j<(int)m && out_of_range[i][j]) ++j;
    --j;
    while (j>=0 && out_of_range[i][j])
    {
      points[i][j] = points[i][j+1];
      colors[i][j] = colors[i][j+1];
      --j;
    }
    for (j=1; j<(int)m; ++j)
    {
      if (out_of_range[i][j])
      {
        points[i][j] = points[i][j-1];
        colors[i][j] = colors[i][j-1];
      }
    }
  }

  uint32_t m = g_road_rec->GetData()[0].GetNumPoints();
  points_gl_array = new Utils::Vector3D[n*m*4];
  colors_gl_array = new Utils::ColorRGB[n*m*4];
  numGlPoints = 0;

  for (uint32_t i=1; i<n; ++i)
  {
    uint32_t m = g_road_rec->GetData()[i].GetNumPoints();
    uint32_t j = 0;

    while (j<m && out_of_range[i][j]) ++j;
    while (m>0 && out_of_range[i][m-1]) --m;

    for (++j; j<m; ++j)
    {
      points_gl_array[numGlPoints++] = Utils::Vector3D(points[i][j].x, points[i][j].y, points[i][j].z);
      points_gl_array[numGlPoints++] = Utils::Vector3D(points[i-1][j].x, points[i-1][j].y, points[i-1][j].z);
      points_gl_array[numGlPoints++] = Utils::Vector3D(points[i-1][j-1].x, points[i-1][j-1].y, points[i-1][j-1].z);
      points_gl_array[numGlPoints++] = Utils::Vector3D(points[i][j-1].x, points[i][j-1].y, points[i][j-1].z);

      numGlPoints -= 4;
      colors_gl_array[numGlPoints++] = colors[i][j];
      colors_gl_array[numGlPoints++] = colors[i-1][j];
      colors_gl_array[numGlPoints++] = colors[i-1][j-1];
      colors_gl_array[numGlPoints++] = colors[i][j-1];
    }
  }
  camera_pos.y = 3000.0;
  camera_pos.z = 5000.0;
  camera_at = Utils::Vector3D(0, 0, 0);
  return true;
}

void SGApp::UpdateData()
{
}

void SGApp::Render()
{
  auto start = std::chrono::system_clock::now();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(
    camera_pos.x, camera_pos.y, camera_pos.z,
    camera_at.x, camera_at.y, camera_at.z,
    0, 0, 1
 );

  DrawGrids();
  DrawCameraTarGet();

  if (g_road_rec)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(3, GL_DOUBLE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glColorPointer(3, GL_DOUBLE, 0, NULL);

    glDrawArrays(GL_QUADS, 0, numGlPoints);
  }
  std::chrono::duration<double> diff = std::chrono::system_clock::now()-start;
    std::cout << "FPS: " << 1.0/diff.count() << std::endl;
}

void SGApp::Zoom(float dz)
{
  dz *= 100;
  Utils::Vector3D temp(camera_at - camera_pos);
  double dis = temp.GetMagnitude();
  if (dis > dz)
  {
    camera_pos.x += temp.x*dz/dis;
    camera_pos.y += temp.y*dz/dis;
    camera_pos.z += temp.z*dz/dis;
  }
  DEBUG_PRINT("camera pos: %.2lf %.2lf %.2lf",
            camera_pos.x, camera_pos.y, camera_pos.z);
}

static void Rotatecamera_pos(const double x,const double y)
{
  camera_pos.z = camera_pos.z + y;
  Utils::Vector3D temp(camera_pos - camera_at);
  camera_pos.x = camera_at.x + cos(-x)*temp.x - sin(-x)*temp.y;
  camera_pos.y = camera_at.y + sin(-x)*temp.x + cos(-x)*temp.y;
}

void SGApp::RotateCamera(const wxPoint& mouse_mic)
{
  Rotatecamera_pos((double)mouse_mic.x/200, (double)mouse_mic.y*5);
  DEBUG_PRINT("camera pos: %.2lf %.2lf %.2lf",
            camera_pos.x, camera_pos.y, camera_pos.z);
}

void SGApp::DragCamera(const wxPoint& mouse_mic)
{
  Utils::Vector3D relativePos(camera_pos-camera_at);
  Matrix temp_mat;
  temp_mat.LoadIdentity();

  Utils::Vector3D screen_delta_vec(-mouse_mic.x, 0, mouse_mic.y); // 2D screen
  temp_mat.Translate(screen_delta_vec);

  double alpha = RAD2DEG(atan(relativePos.y/relativePos.x));
  if (relativePos.x < 0)
  {
    alpha += PI;
  }
  temp_mat.RotateZ(alpha);
  Utils::Vector3D temp_vec(relativePos);
  temp_vec.z = 0;
  double beta = RAD2DEG(atan(relativePos.z/temp_vec.GetMagnitude()));
  temp_mat.RotateX(beta);
  temp_mat.RotateZ(-alpha);

  Utils::Vector3D delta_vec = temp_mat.GetPosition();
  delta_vec.z = 0;

  camera_pos = camera_pos+delta_vec;
  camera_at = camera_at+delta_vec;

  DEBUG_PRINT("camera pos: %.2lf %.2lf %.2lf",
            camera_pos.x, camera_pos.y, camera_pos.z);
  DEBUG_PRINT("camera at: %.2lf %.2lf %.2lf",
            camera_at.x, camera_at.y, camera_at.z);

  DEBUG_PRINT("Dragging");
}

void SGApp::CreateGLBufferObjects()
{
  if (g_road_rec) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numGlPoints*sizeof(Utils::Vector3D), (float*)points_gl_array, GL_STATIC_DRAW);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, numGlPoints*sizeof(Utils::Vector3D), (float*)colors_gl_array, GL_STATIC_DRAW);

    DEBUG_PRINT("OpenGL buffer objects created!");
  }
}

void SGApp::DeleteGLBufferObjects()
{
  if (vbo)
  {
    glDeleteBuffers(1, &vbo);
  }
  if (cbo)
  {
    glDeleteBuffers(1, &cbo);
  }
}

static Utils::ColorRGB MakeColor(float vmin, float vmax, float v)
{
  Utils::ColorRGB c(1, 1, 1);
  double dv;
  if (v < vmin)
  v = vmin;
  if (v > vmax)
  v = vmax;
  dv = vmax - vmin;
  if (v < (vmin + 0.25 * dv))
  {
    c.r = 0;
    c.g = 4 * (v - vmin) / dv;
  }
  else if (v < (vmin + 0.5 * dv))
  {
    c.r = 0;
    c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
  }
  else if (v < (vmin + 0.75 * dv))
  {
    c.r = 4 * (v - vmin - 0.5 * dv) / dv;
    c.b = 0;
  }
  else
  {
    c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
    c.b = 0;
  }

  return (c);
}

static void DrawGrids (void)
{
  glPushAttrib( GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glDisable( GL_LIGHTING);
  glDisable( GL_TEXTURE_2D);

  glBegin( GL_LINES);
  for (float i = -10000.0f; i < 10000.0f; i+=GRID_SIZE)
  {
    glColor3f( 0.8f, 0.8f, 0.8f);

    if (i == 0.0f)
    {
      glColor3f( 0.0f, 0.0f, 1.0f);
    }
    glVertex3f(i, 10000.0f, 0.0f);
    glVertex3f(i, -10000.0f, 0.0f);

    if (i == 0.0f)
    {
      glColor3f( 1.0f, 0.0f, 0.0f);
    }
    glVertex3f(5000.0f, i, 0.0f);
    glVertex3f(-5000.0f, i, 0.0f);
  }
  glEnd();

  glPointSize( 10);
  glBegin( GL_POINTS);
  glColor3f( 1.0f, 0.0f, 0.0f);
  glVertex3f(GRID_SIZE, 0.0f, 0.0f);
  glColor3f( 0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, GRID_SIZE, 0.0f);
  glEnd();
  glPointSize( 1);

  glPopAttrib();
}

static void DrawCameraTarGet()
{
  glPointSize(10);
  glBegin(GL_POINTS);
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(camera_at.x, camera_at.y, camera_at.z);
  glEnd();
  glPointSize(1);
}
