#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "utils.h"

#define MATRIX_SIZE 4

class Matrix
{
public:
  Matrix();
  ~Matrix();

  Matrix operator*(const Matrix& mat) const;
  Matrix& operator=(const Matrix& mat);

  void LoadIdentity();
  void Translate(const Utils::Vector3D& pos);
  void Scale(const Utils::Vector3D& vec);
  // Rotations use angles in degrees
  void RotateX(const double val);
  void RotateY(const double val);
  void RotateZ(const double val);
  void Multiply(const Matrix& mat) { (*this) = mat * (*this); }
  Utils::Vector3D GetPosition() {
    return Utils::Vector3D(data[0][3], data[1][3], data[2][3]);
  }

  double data[MATRIX_SIZE][MATRIX_SIZE];
};

#endif // _MATRIX_H_
