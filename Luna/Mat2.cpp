#include "Mat2.h"

namespace Luna
{
	Mat2::Mat2() {}
	Mat2::Mat2(real rad)
	{
		// constractor to create a rotation matrix from an angle in radians
		real c = cos(rad);
		real s = sin(rad);

		// Set up the rotation matrix: 
		// [ cos(a)  -sin(a) ] 
		// [ sin(a)   cos(a) ] 
		this->m00 = c; this->m01 = -s;
		this->m10 = s; this->m11 = c;
	}

	// Constructor to create a matrix with specified elements
	// [ a  b ]
	// [ c  d ]
	Mat2::Mat2(real a, real b, real c, real d) : m00(a), m01(b), m10(c), m11(d) {}

	Mat2::~Mat2() {}

	void Mat2::Set(real radians)
	{
		// Set the matrix to a rotation matrix for the given angle in radians
		real c = cos(radians);
		real s = sin(radians);

		// Set up the rotation matrix: 
		// [ cos(a)  -sin(a) ] 
		// [ sin(a)   cos(a) ] 
		this->m00 = c; this->m01 = -s;
		this->m10 = s; this->m11 = c;
	}

	// Return a new matrix with the absolute values of all elements
	Mat2 Mat2::Abs() const { return Mat2(abs(this->m00), abs(this->m01), abs(this->m10), abs(this->m11)); }

	// Return the X axis of the matrix (first column)
	Vector2 Mat2::AxisX() const { return Vector2(this->m00, this->m10); }

	// Return the Y axis of the matrix (second column)
	Vector2 Mat2::AxisY() const { return Vector2(this->m01, this->m11); }

	// Return the transpose of the matrix (rows become columns and vice versa)
	Mat2 Mat2::Transpose() const { return Mat2(this->m00, this->m10, this->m01, this->m11); }

	// Multiply the matrix by a vector
	// [ m00  m01 ] * [ v.x ] = [ m00*v.x + m01*v.y ]
	// [ m10  m11 ]   [ v.y ]   [ m10*v.x + m11*v.y ]
	const Vector2 Mat2::operator*(const Vector2& v) const { return Vector2(this->m00 * v.x + this->m01 * v.y,  this->m10 * v.x + this->m11 * v.y); }

	// Multiply this matrix by another matrix
	// [ m00  m01 ] * [ v.m00  v.m01 ] = [ m00*v.m00 + m01*v.m10   m00*v.m01 + m01*v.m11 ]
	// [ m10  m11 ]   [ v.m10  v.m11 ]   [ m10*v.m00 + m11*v.m10   m10*v.m01 + m11*v.m11 ]
	const Mat2 Mat2::operator*(const Mat2& v) const
	{
		return Mat2(m[0][0] * v.m[0][0] + m[0][1] * v.m[1][0],
					m[0][0] * v.m[0][1] + m[0][1] * v.m[1][1],
					m[1][0] * v.m[0][0] + m[1][1] * v.m[1][0],
					m[1][0] * v.m[0][1] + m[1][1] * v.m[1][1]);
	}
}