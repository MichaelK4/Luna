#include "Mat2.h"

namespace Luna
{
	Mat2::Mat2() {}
	Mat2::Mat2(real radians)
	{
		real c = cos(radians);
		real s = sin(radians);

		m00 = c; m01 = -s;
		m10 = s; m11 = c;
	}

	Mat2::Mat2(real a, real b, real c, real d) : m00(a), m01(b), m10(c), m11(d) {}

	Mat2::~Mat2() {}

	void Mat2::Set(real radians)
	{
		real c = cos(radians);
		real s = sin(radians);

		m00 = c; m01 = -s;
		m10 = s; m11 = c;
	}

	Mat2 Mat2::Abs() const
	{
		return Mat2(abs(m00), abs(m01), abs(m10), abs(m11)); 
	}

	Vector2 Mat2::AxisX() const
	{
		return Vector2(m00, m10);
	}

	Vector2 Mat2::AxisY() const
	{
		return Vector2(m01, m11);
	}

	Mat2 Mat2::Transpose() const
	{
		return Mat2(m00, m10, m01, m11);
	}

	const Vector2 Mat2::operator*(const Vector2& v) const
	{
		return Vector2(m00 * v.x + m01 * v.y, m10 * v.x + m11 * v.y);
	}

	const Mat2 Mat2::operator*(const Mat2& v) const
	{
		return Mat2(m[0][0] * v.m[0][0] + m[0][1] * v.m[1][0],
					m[0][0] * v.m[0][1] + m[0][1] * v.m[1][1],
					m[1][0] * v.m[0][0] + m[1][1] * v.m[1][0],
					m[1][0] * v.m[0][1] + m[1][1] * v.m[1][1]);
	}
}
