#include "Collider.h"

Collider::Collider(const Vector2& position, const Vector2& size)
	: position(position), size(size)
{}


bool Collider::IsColliding(const Collider& other) const
{
	return position.x < other.position.x + other.size.x &&
		position.x + size.x > other.position.x &&
		position.y < other.position.y + other.size.y &&
		position.y + size.y > other.position.y;
}

