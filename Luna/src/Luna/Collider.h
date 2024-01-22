#pragma once
#include "Vector2.h"
class Collider
{
public:

	Vector2 position;
	Vector2 size;

	Collider(const Vector2& position, const Vector2& size);

	bool IsColliding(const Collider& other) const;
};

