#ifndef _UTILS_H_
#define _UTILS_H_

#include "libs.h"

#define MAXSIZE 65536
#define DEG2RAD(x) (x*PI/180)
#define RAD2DEG(x) (x*180/PI)
#define SIND(x) (sin(DEG2RAD(x)))
#define COSD(x) (cos(DEG2RAD(x)))
#define TAND(x) (tan(DEG2RAD(x)))

inline double ABS(double x) {
  if (x<0)
    return -x;
  return x;
}

namespace Utils
{
  class Vector2D
  {
  public:
    double x, y;

    Vector2D() : x(0), y(0) {}
    Vector2D(double x, double y) : x(x), y(y) {}
    ~Vector2D() {}

    double GetX() { return x; }
    double GetY() { return y; }
  };

  class Vector3D
  {
  public:
    double x, y, z;

    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double newx, double newy, double newz) : x(newx), y(newy), z(newz) {}
    Vector3D(const Vector3D& src) : x(src.x), y(src.y), z(src.z) {}
    ~Vector3D() {}

    Vector3D& operator=(Vector3D t) {
      this->x = t.x;
      this->y = t.y;
      this->z = t.z;
      return *this;
    }
    Vector3D operator+(const Vector3D& t) {
      Vector3D vec(x + t.x, y + t.y, z + t.z);
      return vec;
    }
    Vector3D operator-(const Vector3D& t) {
      Vector3D vec(x - t.x, y - t.y, z - t.z);
      return vec;
    }

    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }
    double GetMagnitude() const {
      double xx = x*x;
      double yy = y*y;
      double zz = z*z;
      return sqrt(xx + yy + zz);
    }

    void SetX(double newx) { x = newx; }
    void SetY(double newy) { y = newy; }
    void SetZ(double newz) { z = newz; }
  };

  class ColorRGB
  {
  public:
    double r;
    double g;
    double b;

    ColorRGB() {}
    ColorRGB(float newr, float newg, float newb) : r(newr), g(newg), b(newb) {}
    ~ColorRGB() {}
  };

  bool PrefixMatched(const char* prefix, const char* str);
  void Delay(const int64_t micros);

  void FilterData(const uint32_t data_size, double* data_in, double** out_data);
};

#endif // _UTILS_H_
