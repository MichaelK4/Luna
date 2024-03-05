#include "Vector2.h"

namespace Luna
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

	Vector2::Vector2() : x(0.0f), y(0.0f) {} // default constructor

	Vector2::Vector2(real x, real y) : x(x), y(y) {} // constructor

	Vector2::Vector2(const Vector2& otherVec) : x(otherVec.x), y(otherVec.y) {} // copy constructor

	Vector2::~Vector2() {} // destructor

	// operator+ overloading
	Vector2 Vector2::operator+(const Vector2& otherVec) const
	{
		return Vector2(this->x + otherVec.x, this->y + otherVec.y);
	}

	// operator+ overloading for adding scalar
	Vector2 Vector2::operator+(real scalar) const
	{
		return Vector2(this->x + scalar, this->y + scalar);
	}

	// operator+= overloading
	Vector2 Vector2::operator+=(const Vector2& otherVec)
	{
		return Vector2(this->x += otherVec.x, this->y += otherVec.y);
	}

	// operator- overloading
	Vector2 Vector2::operator-(const Vector2& otherVec) const
	{
		return Vector2(this->x - otherVec.x, this->y - otherVec.y);
	}

	// operator- overloading for negative vector
	Vector2 Vector2::operator-() const
	{
		return Vector2(-this->x, -this->y);
	}

	// operator-= overloading
	Vector2 Vector2::operator-=(const Vector2& otherVec)
	{
		return Vector2(this->x -= otherVec.x, this->y -= otherVec.y);
	}

	// operator* overloading
	Vector2 Vector2::operator*(const real& scalar) const
	{
		return Vector2(this->x * scalar, this->y * scalar);
	}

	// operator*= overloading
	Vector2 Vector2::operator*=(const real& scalar)
	{
		return Vector2(this->x *= scalar, this->y *= scalar);
	}

	// operator/ overloading
	Vector2 Vector2::operator/(const real& scalar) const
	{
		return Vector2(this->x / scalar, this->y / scalar);
	}

	// operator/= overloading
	Vector2 Vector2::operator/=(const real& scalar)
	{
		return Vector2(this->x /= scalar, this->y /= scalar);
	}

	// operator== overloading
	bool Vector2::operator==(const Vector2& otherVec) const
	{
		return (this->x == otherVec.x && this->y == otherVec.y);
	}

	// operator!= overloading
	bool Vector2::operator!=(const Vector2& otherVec) const
	{
		return (this->x != otherVec.x || this->y != otherVec.y);
	}

	// operator= overloading
	Vector2 Vector2::operator=(const Vector2& otherVec)
	{
		if (this != &otherVec)
		{
			this->x = otherVec.x;
			this->y = otherVec.y;
		}
		return *this;
	}

	// set position
	void Vector2::Set(real new_x, real new_y)
	{
		x = new_x;
		y = new_y;
	}

	/* dot product - 2D vector only
	dot product is the cosine of the angle
	between two vectors */
	real Vector2::Dot(const Vector2& v, const Vector2& otherVec) const
	{
		return (v.x * otherVec.x + v.y * otherVec.y);
	}

	/* cross product - 2D vector only
	cross product is the sine of the angle
	between two vectors */
	real Vector2::Cross(const Vector2& v, const Vector2& otherVec) const
	{
		return (v.x * otherVec.y - v.y * otherVec.x);
	}

	// cross product with scalar
	Vector2 Vector2::Cross(const Vector2& otherVec, real s) const
	{
		return Vector2(s * otherVec.y, -s * otherVec.x);
	}

	// cross product with scalar
	Vector2 Vector2::Cross(real s, const Vector2& otherVec) const
	{
		return Vector2(-s * otherVec.y, s * otherVec.x);
	}

	// Normalise the vector
	void Vector2::Normalise()
	{
		real len = Length();
		if (len > EPSILON)
		{
			real invlen = 1.0f / len;
			this->x *= invlen;
			this->y *= invlen;
		}
	}

	// Maximum value between two vectors
	Vector2 Vector2::Max(const Vector2& otherVec) const
	{
		return Vector2(std::max(this->x, otherVec.x), std::max(this->y, otherVec.y));
	}

	// Minimum value between two vectors
	Vector2 Vector2::Min(const Vector2& otherVec) const
	{
		return Vector2(std::min(this->x, otherVec.x), std::min(this->y, otherVec.y));
	}

	// Get the length of the vector
	real Vector2::Length() const
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	// Get the squared length of the vector
	real Vector2::LengthSqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	void Vector2::Rotate(real radians)
	{
		real c = cos(radians);
		real s = sin(radians);

		real xp = this->x * c - this->y * s;
		real yp = this->x * s + this->y * c;

		this->x = xp;
		this->y = yp;
	}

	// Get the distance between two vectors
	real Vector2::Distance(const Vector2& otherVec) const
	{
		real dx = this->x - otherVec.x;
		real dy = this->y - otherVec.y;
		return sqrt(dx * dx + dy * dy);
	}

	// Get the squared distance between two vectors
	real Vector2::DistanceSqrt(const Vector2& otherVec) const
	{
		real dx = this->x - otherVec.x;
		real dy = this->y - otherVec.y;
		return (dx * dx + dy * dy);
	}

	//math functions
	real Random(real a, real b)
	{
		real rnd = (real)rand() / (real)RAND_MAX;
		rnd = (a - b) * rnd + b;
		return rnd;
	}

	// check if two real numbers are equal
	bool Equal(real a, real b)
	{
		return fabs(a - b) <= EPSILON;
	}

	// round a real number to the nearest integer
	int Round(real a)
	{
		return (int)(a + 0.5f);
	}

	real sqr(real a)
	{
		return a * a;
	}

	real Clamp(real val, real min, real max)
	{
		if (val < min)
			return min;
		else if (val > max)
			return max;
		else
			return val;
	}

	bool BiasGreaterThan(real a, real b)
	{
		const real k_biasRelative = 0.95f;
		const real k_biasAbsolute = 0.01f;
		return a >= b * k_biasRelative + a * k_biasAbsolute;
	}
}
