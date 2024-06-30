#ifndef MAT2_H
#define MAT2_H
#include "Vector2.h"

namespace Luna
{
	class Mat2
	{
	public:
		union
		{
			struct
			{
				real m00, m01;
				real m10, m11;
			};
			real m[2][2];
			real v[4];
		};

		Mat2();
		Mat2(real radians);
		Mat2(real a, real b, real c, real d);
		~Mat2();

		void Set(real radians);
		Mat2 Abs() const;
		Vector2 AxisX() const;
		Vector2 AxisY() const;
		Mat2 Transpose() const;

		const Vector2 operator*(const Vector2& v) const;
		const Mat2 operator*(const Mat2& v) const;
	};
}

#endif // !MAT2_H

