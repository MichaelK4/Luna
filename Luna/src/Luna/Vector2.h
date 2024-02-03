#ifndef VECTOR2_H
#define VECTOR2_H

#include <functional> // for std::hash
#include "EngineMath.h" 
class Vector2
{
public:
	real x; // x value of the vector
	real y; // y value of the vector

	static const Vector2 Zero; // (0, 0) vector 

	Vector2(); // default constructor
	Vector2(real x, real y); // constructor
	Vector2(const Vector2& otherVec); // copy constructor
	~Vector2(); // destructor

	// operator+ overloading for adding vector
	Vector2 operator+(const Vector2& otherVec) const;

	// operator+ overloading for adding scalar
	Vector2 operator+(real scalar) const;

	// operator+= overloading for adding vector
	Vector2 operator+=(const Vector2& otherVec);

	// operator- overloading for subtracting scalar
	Vector2 operator-(const Vector2& otherVec) const;

	// operator- overloading for negative vector
	Vector2 operator-() const;

	// operator-= overloading for subtracting vector
	Vector2 operator-=(const Vector2& otherVec);

	// operator* overloading for multiplying vector with scalar
	Vector2 operator*(const real& scalar) const;

	// operator*= overloading for multiplying vector with scalar
	Vector2 operator*=(const real& scalar);
	 
	// operator/ overloading for dividing vector with scalar
	Vector2 operator/(const real& scalar) const;

	// operator/= overloading for dividing vector with scalar
	Vector2 operator/=(const real& scalar);

	// operator== overloading for comparing vector
	bool operator==(const Vector2& otherVec) const;

	// operator!= overloading for comparing vector
	bool operator!=(const Vector2& otherVec) const;

	// operator= overloading for assigning vector
	Vector2 operator=(const Vector2& otherVec);

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

	// set the vector values
	void Set(real x, real y);

	/* dot product - 2D vector only
	dot product is the cosine of the angle
	between two vectors */
	real Dot(const Vector2& otherVec) const;

	/* cross product - 2D vector only
	cross product is the sine of the angle
	between two vectors */
	real Cross(const Vector2& otherVec) const;

	// cross product with scalar 
	Vector2 Cross(const Vector2& otherVec, real s) const;

	// cross product with scalar 
	Vector2 Cross(real s, const Vector2& otherVec) const;

	// Normalise the vector to unit vector
	void Normalise();

    // Maximum value between two vectors 
	Vector2 Max(const Vector2& otherVec) const;

	// Minimum value between two vectors
	Vector2 Min(const Vector2& otherVec) const;

	// Get the length of the vector 
	real Length() const;

	// Get the squared length of the vector 
	real LengthSqrt() const;

	// rotate the vector by radians 
	void Rotate(real radians);

	// Get the distance between two vectors 
	real Distance(const Vector2& otherVec) const;

	// Get the squared distance between two vectors
	real DistanceSqrt(const Vector2& otherVec) const;
};

#endif // !VECTOR2_H