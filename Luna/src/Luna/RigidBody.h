#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "EngineMath.h"
class RigidBody
{
public:

	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	RigidBody(BodyShape shape, const Vector2& startPosition, const Vector2& startVelocity);

	// updtae position based on velocity
	void Update(float deltaTime);

};

#endif // !RIGID_BODY_H
