#include "Libs.h"

namespace Luna
{
	Manifold::Manifold() {} 
	Manifold::Manifold(RigidBody* a, RigidBody* b) : A(a), B(b) {} 
	Manifold::~Manifold() {}

	void Manifold::Solve()
	{
		Dispatch[A->shape->GetType()][B->shape->GetType()](this, A, B);
	}

	void Manifold::Init()
	{
		// Calculate average restitution
		e = std::min(A->GetRestitution(), B->GetRestitution());

		// Calculate static and dynamic friction
		staticFriction = std::sqrt(A->GetStaticFriction() * A->GetStaticFriction());
		dynamicFriction = std::sqrt(A->GetDynamicFriction() * A->GetDynamicFriction());

		for (uint i = 0; i < contactCount; i++)
		{
			// Calculate radius from center of mass to contact - is the vector from the center of mass to the contact point
			Vector2 radA = contacts[i] - A->position;
			Vector2 radB = contacts[i] - B->position;

			// Calculate the relative velocity of the bodies at the contact point
			Vector2 radV = B->velocity + Cross(B->AngularVelocity, radB)
				- A->velocity - Cross(A->AngularVelocity, radA);

			// Calculate the relative velocity in terms of the normal direction
			// the normal component of the relative velocity
			if (radV.LengthSqr() < (GRAVITY * deltaTime).LengthSqr() + EPSILON)
				e = 0.0f;
		}
	}

	void Manifold::ApplyImpulse()
	{
		// Early out and positional correction if both objects have infinite mass
		if (Equal(A->InverseMass + B->InverseMass, 0))
		{
			InfiniteMassCorrection();
			return;
		}

		for (uint i = 0; i < contactCount; i++)
		{
			// Calculate radius from center of mass to contact - is the vector from the center of mass to the contact point
			Vector2 radA = contacts[i] - A->position;
			Vector2 radB = contacts[i] - B->position;

			Vector2 radV = B->velocity + Cross(B->AngularVelocity, radB)
				- A->velocity - Cross(A->AngularVelocity, radA); // Relative velocity

			real contactVel = Dot(radV, normal); // the normal component of the relative velocity

			if (contactVel > 0) // if the objects are moving away from each other we don't need to resolve the collision
				return;

			real radACrossN = Cross(radA, normal);
			real radBCrossN = Cross(radB, normal);
			real invMassSum = A->InverseMass + B->InverseMass +
				Sqr(radACrossN) * A->InverseInertia +
				Sqr(radBCrossN) * B->InverseInertia; // the denominator of the impulse calculation


			// the impulse scalar
			real j = -(1.0f + e) * contactVel;
			j /= invMassSum;
			j /= (real)contactCount;

			// Apply the impulse to the bodies
			Vector2 impulse = normal * j;
			A->Impulse(-impulse, radA);
			B->Impulse(impulse, radB);


			radV = B->velocity + Cross(B->AngularVelocity, radB)
				- A->velocity - Cross(A->AngularVelocity, radA); // Friction impulse

			Vector2 tangent = radV - (normal * Dot(radV, normal)); // the tangent component of the relative velocity
			tangent.Normalise(); // normalize the tangent vector

			real jt = -Dot(radV, tangent); // the magnitude of the tangent component of the relative velocity
			jt /= invMassSum;
			jt /= (real)contactCount;

			if (Equal(jt, 0.0f)) // if the tangent impulse is 0 we don't need to resolve the collision
				return;

			// Coulomb's law - friction magnitude
			Vector2 frictionImpulse;
			if (std::abs(jt) < j * staticFriction)
				frictionImpulse = tangent * jt;
			else
				frictionImpulse = tangent * -j * dynamicFriction;

			// Apply the friction impulse to the bodies
			A->Impulse(-frictionImpulse, radA);
			B->Impulse(frictionImpulse, radB);
		}
	}

	void Manifold::PositionalCorrection()
	{
		const real k_slop = 0.05f; // Penetration allowance - usually 0.01 to 0.1 is good
		const real percent = 0.4f; // Penetration percentage to correct - usually 20% to 80%

		Vector2 correction = normal * (std::max(penetration - k_slop, 0.0f) / (A->InverseMass + B->InverseMass)) * percent;
		A->position -= correction * A->InverseMass;
		B->position += correction * B->InverseMass;
	}

	void Manifold::InfiniteMassCorrection()
	{
		A->velocity.Set(0.0f, 0.0f);
		B->velocity.Set(0.0f, 0.0f);
	}
}