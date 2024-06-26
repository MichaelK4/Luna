#ifndef VECTOR2_H
#define VECTOR2_H
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional> // for std::hash

typedef float real; // Real is a floating point number that represents a real number.
//typedef unsigned int uint; // Unsigned int is a data type that represents an integer that is always positive.
typedef size_t uint; // Size_t is a data type that represents the size of an object in bytes.
typedef unsigned char byte; // Unsigned char is a data type that represents a character that is always positive.

/*Pi is a mathematical constant that represents
the ratio of a circle's circumference to its diameter.*/
const real PI = 3.14159265358979323846f;
// Epsilon is a small number that is used to compare floating point numbers.
const real EPSILON = 0.0001f;
// Real gravity is the force that attracts a body toward the center of the earth.
const real REAL_GRAVITY = 9.8f;

namespace Luna
{
	class Vector2
	{
	public:
		union
		{
			real m[1][1];
			real v[2];

			struct
			{
				real x;
				real y;
			};
		};
		Vector2(); // default constructor 
		Vector2(real x, real y); // constructor 
		Vector2(const Vector2& oVec); // copy constructor 
		~Vector2(); // destructor

		// set the vector values
		void Set(real x, real y);

		// Normalise the vector to unit vector
		void Normalise();

		// Get the length of the vector 
		real Length() const;

		// Get the squared length of the vector 
		real LengthSqr() const;

		// rotate the vector by radians 
		void Rotate(real radians);

		// operator+ overloading for adding vector
		Vector2 operator+(const Vector2& v);

		// operator+ overloading for adding scalar
		Vector2 operator+(real scalar);

		// operator+= overloading for adding vector
		Vector2 operator+=(const Vector2& v);

		// operator- overloading for subtracting scalar
		Vector2 operator-(const Vector2& v);

		// operator-= overloading for subtracting vector
		Vector2 operator-=(const Vector2& v);

		// operator- overloading for negative vector
		Vector2 operator-();

		// operator* overloading for multiplying vector with scalar
		Vector2 operator*(const real& scalar) const;

		// operator*= overloading for multiplying vector with scalar
		Vector2 operator*=(const real& scalar);

		// operator/ overloading for dividing vector with scalar
		Vector2 operator/(const real& scalar) const;

		// operator/= overloading for dividing vector with scalar
		Vector2 operator/=(const real& scalar);

		//// operator== overloading for comparing vector
		//bool operator==(const Vector2& oVec) const;

		//// operator!= overloading for comparing vector
		//bool operator!=(const Vector2& oVec) const;

		//// operator= overloading for assigning vector
		//Vector2 operator=(const Vector2& oVec);

		/* Jenkins one - at - a - time hash function algorithm
		for hashing the vector values to a unique hash code value
		https://en.wikipedia.org/wiki/Jenkins_hash_function */
		std::size_t hash_code() const
		{
			std::hash<real> hasher;
			std::size_t hashX = hasher(x);
			std::size_t hashY = hasher(y);
			return hashX ^ (hashY + 0x9e3779b9 + (hashX << 6) + (hashX >> 2));
		}
	};

	// Maximum value between two vectors 
	Vector2 Max(const Vector2& v, const Vector2& oVec);

	// Minimum value between two vectors
	Vector2 Min(const Vector2& v, const Vector2& oVec);

	// Get the squared distance between two vectors
	real DistSqr(const Vector2& v, const Vector2& oVec);

	/* dot product - 2D vector only
		dot product is the cosine of the angle
		between two vectors */
	real Dot(const Vector2& v, const Vector2& oVec);

	/* cross product - 2D vector only
	cross product is the sine of the angle
	between two vectors */
	real Cross(const Vector2& v, const Vector2& oVec);

	// cross product with scalar 
	Vector2 Cross(const Vector2& v, real scalar);

	// cross product with scalar 
	Vector2 Cross(real scalar, const Vector2& v);

	// Equal is a function that compares two floating point numbers and returns true if they are equal.
	bool Equal(real a, real b);

	// Random is a function that returns a random number between two floating point numbers.
	real Random(real a, real b);

	// Round is a function that rounds a floating point number to the nearest integer.
	int Round(real a);

	// Sqr is a function that returns the square of a floating point number.
	real Sqr(real a);

	real Clamp(real val, real min, real max); // Clamp is a function that clamps a floating point number between two values.

	/* Bias greater than is a function that compares
	two floating point numbers and returns true if the
	first number is greater than the second number. */
	bool BiasGreaterThan(real a, real b);


	/* Acceleration
	*  F = m * a
	*  a = F / m
	*/

	/* Explicit Euler
	*  x = x + v * dt
	*  v = v + a * dt
	*/

	/* Semi-Implicit Euler
	*  v = v + a * dt
	*  x = x + v * dt
	*/
}
// Delta time is the amount of time that has passed since the last frame.
const float deltaTime = 1.0f / 60.0f;
// Gravity scale is a value that is used to scale the force of gravity.
const real GRAVITY_S = 5.0f;
// Gravity is a vector that represents the force of gravity.
const Luna::Vector2 GRAVITY(0.0f, 9.8f * GRAVITY_S);

#endif // !VECTOR2_H


