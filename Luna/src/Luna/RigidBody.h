#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "Vector2.h"
class RigidBody
{
public:

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	RigidBody(const Vector2& startPosition, const Vector2& startVelocity);

	// updtae position based on velocity
	void Update(float deltaTime);

};

#endif // !RIGID_BODY_H
