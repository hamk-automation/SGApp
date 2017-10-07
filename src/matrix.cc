#include "matrix.h"

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

Matrix Matrix::operator*(const Matrix& mat) const
{
  Matrix result_mat;
  for (uint32_t i=0; i<MATRIX_SIZE; ++i)
  {
    for (uint32_t j=0; j<MATRIX_SIZE; ++j)
    {
      result_mat.data[i][j] = 0;
      for (uint32_t k=0; k<MATRIX_SIZE; ++k) {
        result_mat.data[i][j] += this->data[i][k] * mat.data[k][j];
      }
    }
  }
  return result_mat;
}

Matrix& Matrix::operator=(const Matrix& mat)
{
  for (uint32_t i=0; i<MATRIX_SIZE; ++i)
  {
    for (uint32_t j=0; j<MATRIX_SIZE; ++j)
    {
      this->data[i][j] = mat.data[i][j];
    }
  }
  return *this;
}

void Matrix::LoadIdentity()
{
  for (uint32_t i=0; i<MATRIX_SIZE; ++i)
  {
    for (uint32_t j=0; j<MATRIX_SIZE; ++j)
    {
      data[i][j] = 0.0;
    }
    data[i][i] = 1.0;
  }
}

void Matrix::Translate(const Utils::Vector3D& pos)
{
  Matrix translate_mat;
  translate_mat.LoadIdentity();
	translate_mat.data[0][3] = pos.x;
	translate_mat.data[1][3] = pos.y;
	translate_mat.data[2][3] = pos.z;
  *this = translate_mat * (*this);
}

void Matrix::Scale(const Utils::Vector3D& vec)
{
  Matrix scale_mat;
  scale_mat.LoadIdentity();
	scale_mat.data[0][0] = vec.x;
	scale_mat.data[1][1] = vec.y;
	scale_mat.data[2][2] = vec.z;
  *this = scale_mat * (*this);
}

void Matrix::RotateX(const double val)
{
  double sin_value = sin(DEG2RAD(val));
	double cos_value = cos(DEG2RAD(val));
  Matrix rotate_mat;
  rotate_mat.LoadIdentity();
	rotate_mat.data[1][1] = cos_value;
	rotate_mat.data[1][2] = sin_value;
	rotate_mat.data[2][1] = -sin_value;
	rotate_mat.data[2][2] = cos_value;
  *this = rotate_mat * (*this);
}

void Matrix::RotateY(const double val)
{
  double sin_value = sin(DEG2RAD(val));
	double cos_value = cos(DEG2RAD(val));
  Matrix rotate_mat;
  rotate_mat.LoadIdentity();
	rotate_mat.data[0][0] = cos_value;
	rotate_mat.data[0][2] = sin_value;
	rotate_mat.data[2][0] = -sin_value;
	rotate_mat.data[2][2] = cos_value;
  *this = rotate_mat * (*this);
}

void Matrix::RotateZ(const double val)
{
  double sin_value = sin(DEG2RAD(val));
	double cos_value = cos(DEG2RAD(val));
  Matrix rotate_mat;
  rotate_mat.LoadIdentity();
	rotate_mat.data[0][0] = cos_value;
	rotate_mat.data[0][1] = -sin_value;
	rotate_mat.data[1][0] = sin_value;
	rotate_mat.data[1][1] = cos_value;
  *this = rotate_mat * (*this);
}
