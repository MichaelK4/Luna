#ifndef MAT2_H
#define MAT2_H
#include "Vector2.h"
class Mat2
{
private:
	real mat2[2][2];
	real vec2[4];
	real m00, m01,
		 m10, m11;
public:
	Mat2();
	Mat2(real radians);
	Mat2(real m00, real m01, real m10, real m11);
	~Mat2();

	const Vector2 operator*(const Vector2& other) const;
	const Mat2 operator*(const Mat2& other) const;

	void Set(real radians);

	Mat2 Abs() const;
	Vector2 AxisX() const;
	Vector2 AxisY() const;

	Mat2 Transpose() const;


};

#endif // MAT2_H

