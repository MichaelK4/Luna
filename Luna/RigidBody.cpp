#include "Libs.h"
#include "Material.h" 


namespace Luna
{
	RigidBody::RigidBody() {}
	RigidBody::RigidBody(Shape* shape, size_t x, size_t y) : shape(shape->Clone(this)), StaticFriction(0.5f), DynamicFriction(0.3f), Restitution(0.2f)
	{
		// Initialize the rigid body with material properties
		shape->rb = this;
		this->position.Set((real)x, (real)y);
		this->velocity.Set(0.0f, 0.0f);
		this->AngularVelocity = 0.0f;
		this->force.Set(0.0f, 0.0f);
		this->Torque = 0.0f;
		this->Orient = Random(-PI, PI); // Set a random initial orientation
		shape->Init(this);
		// Generate a random color for the rigid body, avoiding greyish colors
		do
		{
			r = Random(0.2f, 1.0f);
			g = Random(0.2f, 1.0f);
			b = Random(0.2f, 1.0f);
		} while (r >= 0.6 && r <= 0.9
			  && g >= 0.6 && g <= 0.9
			  && b >= 0.6 && b <= 0.9);
	}

	RigidBody::RigidBody(Shape* shape, size_t x, size_t y, Material* m) : 
		  shape(shape->Clone(this)),
		  StaticFriction(m->getStaticFriction()), 
		  DynamicFriction(m->getDynamicFriction()),
		  Restitution(m->getRestitution()) 
	{
		// Initialize the rigid body with material properties
		shape->rb = this;
		this->material = m; 
		this->position.Set((real)x, (real)y);
		this->velocity.Set(0.0f, 0.0f);
		this->AngularVelocity = 0.0f;
		this->force.Set(0.0f, 0.0f);
		this->Torque = 0.0f;
		this->Orient = Random(-PI, PI); // Set a random initial orientation
		shape->Init(this);
		// Generate a random color for the rigid body, avoiding greyish colors
		do
		{
			r = Random(0.2f, 1.0f);
			g = Random(0.2f, 1.0f);
			b = Random(0.2f, 1.0f);
		} while (r >= 0.6 && r <= 0.9
			  && g >= 0.6 && g <= 0.9
			  && b >= 0.6 && b <= 0.9);
	}

	RigidBody::~RigidBody() 
	{ 
		// Clean up the shape and print destruction message
		if (shape) { delete shape; }
		if(!isCustomMaterial)
			std:: cout << "Body: " << " Material: " << GetMaterial()->getName() << ", got destroyed" << std::endl << std::endl;
		else
			std:: cout << "Body: " << " Material: " << "Custom " << ", got destroyed" << std::endl << std::endl;
	}

	// Add the given force to the rigid body
	void RigidBody::Force(const Vector2& f) { force += f; }

	void RigidBody::Impulse(const Vector2& impulse, const Vector2& contactVec)
	{
		// Apply an impulse to change velocity and angular velocity
		velocity += impulse * InverseMass;
		AngularVelocity += InverseInertia * Cross(contactVec, impulse);
	}

	void RigidBody::SetOrient(real rad)
	{
		// Set the orientation of the rigid body and update the shape
		Orient = rad;
		shape->SetOrient(rad);
	}

	void RigidBody::SetStatic()
	{
		// Make the rigid body static by setting mass and inertia to infinity (inverse to zero)
		InverseMass = 0.0f;
		InverseInertia = 0.0f;
		Mass = 0.0f;
		Inertia = 0.0f;
	}

	void RigidBody::SetStaticFriction(real sf) { this->StaticFriction = sf; }
	void RigidBody::SetDynamicFriction(real df) { this->DynamicFriction = df; }
	void RigidBody::SetRestitution(real r) { this->Restitution = r; }
	void RigidBody::SetMaterial(Material* m)
	{
		// Set the material and update related properties
		this->material = m; 
		this->Restitution = m->getRestitution();  
		this->StaticFriction = m->getStaticFriction();
		this->DynamicFriction = m->getDynamicFriction(); 
	}

	real RigidBody::GetStaticFriction() { return this->StaticFriction; }
	real RigidBody::GetDynamicFriction() { return this->DynamicFriction; }
	real RigidBody::GetRestitution() { return this->Restitution; }
	Material* RigidBody::GetMaterial() { return this->material; }
}