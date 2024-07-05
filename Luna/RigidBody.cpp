#include "Libs.h"
#include "Material.h" 


namespace Luna
{
	RigidBody::RigidBody() {}
	RigidBody::RigidBody(Shape* shape, size_t x, size_t y) : shape(shape->Clone(this)), StaticFriction(0.5f), DynamicFriction(0.3f), Restitution(0.2f)
	{
		shape->rb = this;
		this->position.Set((real)x, (real)y);
		this->velocity.Set(0.0f, 0.0f);
		this->AngularVelocity = 0.0f;
		this->force.Set(0.0f, 0.0f);
		this->Torque = 0.0f;
		this->Orient = Random(-PI, PI);
		shape->Init(this);
		do
		{
			r = Random(0.2f, 1.0f);
			g = Random(0.2f, 1.0f);
			b = Random(0.2f, 1.0f);
		} while (r >= 0.6 && r <= 0.9
			  && g >= 0.6 && g <= 0.9
			  && b >= 0.6 && b <= 0.9);
	}

	RigidBody::RigidBody(Shape* shape, size_t x, size_t y, Material* m) : shape(shape->Clone(this)),
		  StaticFriction(m->getStaticFriction()), 
		  DynamicFriction(m->getDynamicFriction()),
		  Restitution(m->getRestitution()) 
	{
		shape->rb = this;
		this->material = m; 
		this->position.Set((real)x, (real)y);
		this->velocity.Set(0.0f, 0.0f);
		this->AngularVelocity = 0.0f;
		this->force.Set(0.0f, 0.0f);
		this->Torque = 0.0f;
		this->Orient = Random(-PI, PI);
		shape->Init(this);
		do
		{
			r = Random(0.2f, 1.0f);
			g = Random(0.2f, 1.0f);
			b = Random(0.2f, 1.0f);
		} while (r >= 0.6 && r <= 0.9
			  && g >= 0.6 && g <= 0.9
			  && b >= 0.6 && b <= 0.9);
	}

	RigidBody::~RigidBody() { std::cout << "Body deleted" << std::endl; }


	void RigidBody::Force(const Vector2& f)
	{
		force += f;
	}

	void RigidBody::Impulse(const Vector2& impulse, const Vector2& contactVec)
	{
		velocity += impulse * InverseMass;
		AngularVelocity += InverseInertia * Cross(contactVec, impulse);
	}

	void RigidBody::SetOrient(real rad)
	{
		Orient = rad;
		shape->SetOrient(rad);
	}

	void RigidBody::SetStatic()
	{
		InverseMass = 0.0f;
		InverseInertia = 0.0f;
		Mass = 0.0f;
		Inertia = 0.0f;
	}

	void RigidBody::SetStaticFriction(real sf)
	{
		this->StaticFriction = sf;
	}

	void RigidBody::SetDynamicFriction(real df)
	{
		this->DynamicFriction = df;
	}

	void RigidBody::SetRestitution(real r)
	{
		this->Restitution = r;
	}

	void RigidBody::SetMaterial(Material* m)
	{
		this->material = m; 
		this->Restitution = m->getRestitution();  
		this->StaticFriction = m->getStaticFriction();
		this->DynamicFriction = m->getDynamicFriction(); 
	}

	real RigidBody::GetStaticFriction() 
	{
		return this->StaticFriction;
	}

	real RigidBody::GetDynamicFriction()
	{
		return this->DynamicFriction;
	}

	real RigidBody::GetRestitution()
	{
		return this->Restitution;
	}

	Material* RigidBody::GetMaterial() 
	{
		return this->material;
	}
}