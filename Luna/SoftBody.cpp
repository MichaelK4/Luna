#include "SoftBody.h"

namespace Luna
{
	SoftBody::SoftBody() : RigidBody()
	{

	}

	SoftBody::SoftBody(Shape* shape, size_t x, size_t y) : RigidBody(shape, x, y)
	{
		Softness = 0.0f;
		Damping = 0.0f;
	}

	SoftBody::~SoftBody()
	{
	}

	void SoftBody::Update(float dt)
	{
		velocity *= Damping;
		AngularVelocity *= Damping;

	}
}
