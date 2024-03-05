#include "Shape.h"


namespace Luna
{
	RigidBody::RigidBody() {}
	RigidBody::RigidBody(Shape* shape, uint x, uint y): shape(shape->Clone(shape->rb))
	{
		shape->rb = this;
		position.Set((real)x, (real)y);
		velocity.Set(0.0f, 0.0f);
		AngularVelocity = 0.0f;
		force.Set(0.0f, 0.0f);
		Torque = 0.0f;
		Orient = Random(-PI, PI);
		StaticFriction = 0.5f;
		DynamicFriction = 0.3f;
		Restitution = 0.2f;
		shape->Init(shape->rb); 
		r = Random(0.1f, 1.0f);
		g = Random(0.1f, 1.0f);
		b = Random(0.1f, 1.0f);
	}
	RigidBody::~RigidBody() {}


	void RigidBody::Force(const Vector2& f)
	{
		force += f;
	}

	void RigidBody::Impulse(const Vector2& impulse, const Vector2& contactVec)
	{
		velocity += impulse * InverseMass;

		real cross = contactVec.Cross(contactVec, impulse);
		AngularVelocity += InverseInertia * cross;
	}

	void RigidBody::SetOrient(real radians)
	{
		Orient = radians;
		shape->SetOrient(radians);
	}

	void RigidBody::SetStatic()
	{
		InverseMass = 0.0f;
		InverseInertia = 0.0f;
		Mass = 0.0f;
		Inertia = 0.0f;
	}
}
