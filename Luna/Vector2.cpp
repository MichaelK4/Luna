#include "Vector2.h"

namespace Luna
{
	Vector2::Vector2() : x(0.0f), y(0.0f) {}
	Vector2::Vector2(real x, real y) :x(x), y(y) {}
	Vector2::Vector2(const Vector2& oVec) : x(oVec.x), y(oVec.y) {}
	Vector2::~Vector2() {}

	void Vector2::Set(real x, real y)
	{
		this->x = x;
		this->y = y;
	}

	void Vector2::Normalise()
	{
		real len = Length();
		if (len > EPSILON)
		{
			real t = 1.0f / len;
			this->x *= t;
			this->y *= t;
		}
	}

	real Vector2::Length() const
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	// Get the squared length of the vector 
	real Vector2::LengthSqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	// rotate the vector by radians 
	void Vector2::Rotate(real rad)
	{
		real c = cos(rad);
		real s = sin(rad);

		real x_ = this->x * c - this->y * s;
		real y_ = this->x * s + this->y * c;

		this->x = x_;
		this->y = y_;
	}

	// operator+ overloading for adding vector
	Vector2 Vector2::operator+(const Vector2& v)
	{
		return Vector2(this->x + v.x, this->y + v.y);
	}

	// operator+ overloading for adding scalar
	Vector2 Vector2::operator+(real scalar)
	{
		return Vector2(this->x + scalar, this->y + scalar);
	}

	// operator+= overloading for adding vector
	Vector2 Vector2::operator+=(const Vector2& v)
	{
		return Vector2(this->x += v.x, this->y += v.y);
	}

	// operator- overloading for subtracting scalar
	Vector2 Vector2::operator-(const Vector2& v)
	{
		return Vector2(this->x - v.x, this->y - v.y);
	}

	// operator-= overloading for subtracting vector
	Vector2 Vector2::operator-=(const Vector2& v)
	{
		return Vector2(this->x -= v.x, this->y -= v.y);
	}

	// operator- overloading for negative vector
	Vector2 Vector2::operator-()
	{
		return Vector2(-this->x, -this->y);
	}

	// operator* overloading for multiplying vector with scalar
	Vector2 Vector2::operator*(const real& scalar) const
	{
		return Vector2(this->x * scalar, this->y * scalar);
	}

	// operator*= overloading for multiplying vector with scalar
	Vector2 Vector2::operator*=(const real& scalar)
	{
		return Vector2(this->x *= scalar, this->y *= scalar);
	}

	// operator/ overloading for dividing vector with scalar
	Vector2 Vector2::operator/(const real& scalar) const
	{
		return Vector2(this->x / scalar, this->y / scalar);
	}

	// operator/= overloading for dividing vector with scalar
	Vector2 Vector2::operator/=(const real& scalar)
	{
		return Vector2(this->x /= scalar, this->y /= scalar);
	}

	//// operator== overloading for comparing vector
	//bool Vector2::operator==(const Vector2& oVec) const
	//{
	//	return (this->x == otherVec.x && this->y == otherVec.y);
	//}

	//// operator!= overloading for comparing vector
	//bool Vector2::operator!=(const Vector2& oVec) const
	//{
	//	return (this->x != otherVec.x || this->y != otherVec.y);
	//}

	//// operator= overloading for assigning vector
	//Vector2 Vector2::operator=(const Vector2& oVec)
	//{
	//	if (this != &otherVec)
	//	{
	//		this->x = otherVec.x;
	//		this->y = otherVec.y;
	//	}
	//	return *this;
	//}


	// Maximum value between two vectors 
	Vector2 Max(const Vector2& v, const Vector2& oVec)
	{
		return Vector2(std::max(v.x, oVec.x), std::max(v.y, oVec.y));
	}

	// Minimum value between two vectors
	Vector2 Min(const Vector2& v, const Vector2& oVec)
	{
		return Vector2(std::min(v.x, oVec.x), std::min(v.y, oVec.y));
	}

	// Get the squared distance between two vectors
	real DistSqr(const Vector2& v, const Vector2& oVec)
	{
		Vector2 a = v;
		a -= oVec;
		return Dot(a, a);
	}

	/* dot product - 2D vector only
		dot product is the cosine of the angle
		between two vectors */
	real Dot(const Vector2& v, const Vector2& oVec)
	{
		return (v.x * oVec.x + v.y * oVec.y);
	}

	/* cross product - 2D vector only
	cross product is the sine of the angle
	between two vectors */
	real Cross(const Vector2& v, const Vector2& oVec)
	{
		return (v.x * oVec.y - v.y * oVec.x);
	}

	// cross product with scalar 
	Vector2 Cross(const Vector2& v, real scalar)
	{
		return Vector2(scalar * v.y, -scalar * v.x);
	}

	// cross product with scalar 
	Vector2 Cross(real scalar, const Vector2& v)
	{
		return Vector2(-scalar * v.y, scalar * v.x);
	}

	// Equal is a function that compares two floating point numbers and returns true if they are equal.
	bool Equal(real a, real b)
	{
		return std::abs(a - b) <= EPSILON;
	}

	// Random is a function that returns a random number between two floating point numbers.
	real Random(real a, real b)
	{
		real c = (real)rand() / RAND_MAX;
		//c /= RAND_MAX;
		c = (b - a) * c + a;
		return c;
	}

	// Round is a function that rounds a floating point number to the nearest integer.
	int Round(real a)
	{
		return (int)(a + 0.5f);
	}

	// Sqr is a function that returns the square of a floating point number.
	real Sqr(real a)
	{
		return a * a;
	}

	// Clamp is a function that clamps a floating point number between two values.
	real Clamp(real val, real min, real max)
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;
		else
			return val;
	}

	/* Bias greater than is a function that compares
	two floating point numbers and returns true if the
	first number is greater than the second number. */
	bool BiasGreaterThan(real a, real b)
	{
		const real k_biasRelative = 0.95f;
		const real k_biasAbsolute = 0.01f;
		return a >= b * k_biasRelative + a * k_biasAbsolute;
	}
}
