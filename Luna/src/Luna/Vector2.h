#pragma once
#include <functional>
class Vector2
{
public:
	float x;
	float y;

	static const Vector2 Zero;

	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& otherVec);
	~Vector2();

	// operator+ overloading
	Vector2 operator+(const Vector2& otherVec) const;

	// operator- overloading
	Vector2 operator-(const Vector2& otherVec) const;

	// operator- overloading for negative vector
	Vector2 operator-() const;

	// operator* overloading
	Vector2 operator*(const float& scalar) const;

	// operator/ overloading
	Vector2 operator/(const float& scalar) const;

	// operator+= overloading
	Vector2 operator+=(const Vector2& otherVec);

	// operator-= overloading
	Vector2 operator-=(const Vector2& otherVec);

	// operator*= overloading
	Vector2 operator*=(const float& scalar);

	// operator/= overloading
	Vector2 operator/=(const float& scalar);

	// operator== overloading
	bool operator==(const Vector2& otherVec) const;

	// Jenkins one-at-a-time hash function algorithm
	std::size_t hash_code() const
	{
		std::hash<float> hasher;
		std::size_t hashX = hasher(x);
		std::size_t hashY = hasher(y);
		return hashX ^ (hashY + 0x9e3779b9 + (hashX << 6) + (hashX >> 2));
	}

	// operator!= overloading
	bool operator!=(const Vector2& otherVec) const;

	// operator= overloading
	Vector2 operator=(const Vector2& otherVec);

	/* dot product - 2D vector only
	dot product is the cosine of the angle
	between two vectors */
	float Dot(const Vector2& otherVec) const;

	/* cross product - 2D vector only
	cross product is the sine of the angle
	between two vectors */
	float Cross(const Vector2& otherVec) const;

};

