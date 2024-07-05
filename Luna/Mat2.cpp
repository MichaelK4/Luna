#include "Mat2.h"

namespace Luna
{
	Mat2::Mat2() {}
	Mat2::Mat2(real rad)
	{
		real c = cos(rad);
		real s = sin(rad);

		this->m00 = c; this->m01 = -s;
		this->m10 = s; this->m11 = c;
	}

	Mat2::Mat2(real a, real b, real c, real d) : m00(a), m01(b), m10(c), m11(d) {}

	Mat2::~Mat2() {}

	void Mat2::Set(real radians)
	{
		real c = cos(radians);
		real s = sin(radians);

		this->m00 = c; this->m01 = -s;
		this->m10 = s; this->m11 = c;
	}

	Mat2 Mat2::Abs() const
	{
		return Mat2(abs(this->m00), abs(this->m01), abs(this->m10), abs(this->m11));
	}

	Vector2 Mat2::AxisX() const
	{
		return Vector2(this->m00, this->m10);
	}

	Vector2 Mat2::AxisY() const
	{
		return Vector2(this->m01, this->m11);
	}

	Mat2 Mat2::Transpose() const
	{
		return Mat2(this->m00, this->m10, this->m01, this->m11);
	}

	const Vector2 Mat2::operator*(const Vector2& v) const
	{
		return Vector2(this->m00 * v.x + this->m01 * v.y, this->m10 * v.x + this->m11 * v.y);
	}

	const Mat2 Mat2::operator*(const Mat2& v) const
	{
		return Mat2(m[0][0] * v.m[0][0] + m[0][1] * v.m[1][0],
			m[0][0] * v.m[0][1] + m[0][1] * v.m[1][1],
			m[1][0] * v.m[0][0] + m[1][1] * v.m[1][0],
			m[1][0] * v.m[0][1] + m[1][1] * v.m[1][1]);
	}
}