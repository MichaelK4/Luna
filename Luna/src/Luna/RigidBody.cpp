#include "RigidBody.h"


RigidBody::RigidBody(const Vector2& startPosition, const Vector2& startVelocity)
	: position(startPosition), velocity(startVelocity), acceleration(0.0f, 0.0f)
{}

void RigidBody::Update(float deltaTime)
{
	// update velocity based on acceleration
	velocity += acceleration * deltaTime;

	// update position based on velocity
	position += velocity * deltaTime;
}