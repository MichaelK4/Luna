#include "Mat2.h"

Mat2::Mat2(){ }
Mat2::Mat2(real m00, real m01, real m10, real m11) : 
	m00(m00), m01(m01), m10(m10), m11(m11)
{}
Mat2::Mat2(real radians)
{
	real c = cos(radians);
	real s = sin(radians);

	m00 = c; m01 = -s;
	m10 = s; m11 = c;
}

void Mat2::Set(real radians)
{
	real c = cos(radians);
	real s = sin(radians);

	m00 = c; m01 = -s;
	m10 = s; m11 = c;
}

Mat2 Mat2::Abs() const
{
	return Mat2(
		abs(m00), abs(m01),
		abs(m10), abs(m11)
	);
}

// Axis X is the first column of the matrix
Vector2 Mat2::AxisX() const
{
	return Vector2(m00, m10);
}

// Axis Y is the second column of the matrix
Vector2 Mat2::AxisY() const
{
	return Vector2(m01, m11);
}

// Transpose the matrix
Mat2 Mat2::Transpose() const
{
	return Mat2(
		m00, m10,
		m01, m11
	);
}

const Mat2 Mat2::operator*(const Mat2& other) const
{
	return Mat2(
		mat2[0][0] * other.mat2[0][0] + mat2[0][1] * other.mat2[1][0],
		mat2[0][0] * other.mat2[0][1] + mat2[0][1] * other.mat2[1][1],
		mat2[1][0] * other.mat2[0][0] + mat2[1][1] * other.mat2[1][0],
		mat2[1][0] * other.mat2[0][1] + mat2[1][1] * other.mat2[1][1]
	);
}

const Vector2 Mat2::operator*(const Vector2& v) const
{
	return Vector2(
		m00 * v.x + m01 * v.y,
		m10 * v.x + m11 * v.y
	);
}