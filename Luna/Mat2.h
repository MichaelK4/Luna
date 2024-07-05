#ifndef MAT2_H
#define MAT2_H
#include "Vector2.h"

namespace Luna
{
	class Mat2
	{
	public:
		union // 2x2 matrix
		{
			struct
			{
				real m00, m01;
				real m10, m11; 
			};
			real m[2][2]; // 2x2 matrix
			real v[4]; // 1D array
		};

		Mat2(); // identity matrix
		Mat2(real radians); // rotation matrix
		Mat2(real a, real b, real c, real d); // 2x2 matrix
		~Mat2(); // destructor

		void Set(real radians); // set rotation matrix
		Mat2 Abs() const; // absolute matrix
		Vector2 AxisX() const; // x-axis
		Vector2 AxisY() const; // y-axis
		Mat2 Transpose() const; // transpose matrix

		const Vector2 operator*(const Vector2& v) const; // matrix-vector multiplication
		const Mat2 operator*(const Mat2& v) const; // matrix-matrix multiplication
	};
}

#endif // !MAT2_H