#include "Vector2.h"

const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2::Vector2(const Vector2& otherVec) : x(otherVec.x), y(otherVec.y) {}

Vector2::~Vector2() {}

Vector2 Vector2::operator+(const Vector2& otherVec) const
{
	return Vector2(x + otherVec.x, y + otherVec.y);
}

Vector2 Vector2::operator-(const Vector2& otherVec) const
{
	return Vector2(x - otherVec.x, y - otherVec.y);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(const float& scalar) const
{
	return Vector2(x / scalar, y / scalar);
}

Vector2 Vector2::operator+=(const Vector2& otherVec)
{
	x += otherVec.x;
	y += otherVec.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& otherVec)
{
	x -= otherVec.x;
	y -= otherVec.y;
	return *this;
}

Vector2 Vector2::operator*=(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2 Vector2::operator/=(const float& scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}


bool Vector2::operator==(const Vector2& otherVec) const
{
	return (x == otherVec.x && y == otherVec.y);
}

bool Vector2::operator!=(const Vector2& otherVec) const
{
	return (x != otherVec.x || y != otherVec.y);
}

Vector2 Vector2::operator=(const Vector2& otherVec)
{
	if (this != &otherVec)
	{
		x = otherVec.x;
		y = otherVec.y;
	}
	return *this;
}

float Vector2::Dot(const Vector2& otherVec) const
{
	return x * otherVec.x + y * otherVec.y;
}

float Vector2::Cross(const Vector2& otherVec) const
{
	return x * otherVec.y - y * otherVec.x;
}



