#include "Libs.h"

namespace Luna
{
	Manifold::Manifold() {} 
	Manifold::Manifold(RigidBody* a, RigidBody* b) : A(a), B(b) {} 
	Manifold::~Manifold() {}

	// Use a dispatch table to call the appropriate collision detection function
	// based on the shapes of the two colliding bodies
	void Manifold::Solve() { Dispatch[A->shape->GetType()][B->shape->GetType()](this, A, B); }

	void Manifold::Init()
	{
		// Calculate average restitution (bounciness) of the collision
		// We use the minimum of the two bodies' restitution values
		// This ensures a more realistic collision response
		e = std::min(A->GetRestitution(), B->GetRestitution());

		// Calculate combined friction coefficients
		// We use the geometric mean (square root of the product) for both static and dynamic friction
		// This provides a good approximation of the combined friction effect
		staticFriction = std::sqrt(A->GetStaticFriction() * A->GetStaticFriction());
		dynamicFriction = std::sqrt(A->GetDynamicFriction() * A->GetDynamicFriction());

		for (size_t i = 0; i < contactCount; i++)
		{
			// Calculate the vectors from each body's center of mass to the contact point
			Vector2 radA = contacts[i] - A->position;
			Vector2 radB = contacts[i] - B->position;

			// Calculate the relative velocity of the bodies at the contact point
			// This includes both linear and angular velocities
			Vector2 radV = B->velocity + Cross(B->AngularVelocity, radB) -
						   A->velocity - Cross(A->AngularVelocity, radA);

			// Check if the relative velocity is very small (objects are nearly at rest)
			// If so, set restitution to 0 to prevent "jittering" of resting contact
			if (radV.LengthSqr() < (GRAVITY * deltaTime).LengthSqr() + EPSILON)   
				e = 0.0f; // Set restitution to 0 for resting contact
		}
	}

	void Manifold::ApplyImpulse()
	{
		// Special case: both objects have infinite mass (immovable)
		// In this case, we don't apply impulses, but we might need to adjust positions
		if (Equal(A->InverseMass + B->InverseMass, 0))
		{
			InfiniteMassCorrection(); // Adjust positions
			return;
		}

		for (size_t i = 0; i < contactCount; i++)
		{
			// Calculate vectors from center of mass to contact point for each body
			Vector2 radA = contacts[i] - A->position;
			Vector2 radB = contacts[i] - B->position;

			// Calculate relative velocity at the contact point
			Vector2 radV = B->velocity + Cross(B->AngularVelocity, radB)- 
						   A->velocity - Cross(A->AngularVelocity, radA); // Relative velocity

			// Calculate the velocity along the normal direction
			real contactVel = Dot(radV, normal); // the normal component of the relative velocity

			if (contactVel > 0) // if the objects are moving away from each other we don't need to resolve the collision
				return;

			// Calculate the effective mass at the contact point
			real radACrossN = Cross(radA, normal); // the cross product of the radius and the normal
			real radBCrossN = Cross(radB, normal); // the cross product of the radius and the normal
			real invMassSum = A->InverseMass  + B->InverseMass +
			 				  Sqr(radACrossN) * A->InverseInertia +
							  Sqr(radBCrossN) * B->InverseInertia; // the denominator of the impulse calculation

			// Calculate the impulse scalar
			// This uses the coefficient of restitution (e) to determine bounciness
			real j = -(1.0f + e) * contactVel; // the magnitude of the impulse
			j /= invMassSum; // the denominator of the impulse calculation
			j /= (real)contactCount; // the number of contacts

			// Apply the impulse to the bodies
			Vector2 impulse = normal * j;
			A->Impulse(-impulse, radA);
			B->Impulse(impulse, radB);

			// Recalculate relative velocity for friction calculation
			radV = B->velocity + Cross(B->AngularVelocity, radB) - 
				   A->velocity - Cross(A->AngularVelocity, radA); // Friction impulse

			// Calculate the tangent vector (perpendicular to the normal)
			Vector2 tangent = radV - (normal * Dot(radV, normal)); // the tangent component of the relative velocity
			tangent.Normalise(); // normalize the tangent vector

			// Calculate the magnitude of the friction impulse
			real jt = -Dot(radV, tangent); // the magnitude of the tangent component of the relative velocity
			jt /= invMassSum; // the denominator of the impulse calculation
			jt /= (real)contactCount; // the number of contacts

			// Don't apply tiny friction impulses
			if (Equal(jt, 0.0f)) // if the tangent impulse is 0 we don't need to resolve the collision
				return;

			// Coulomb's law - friction magnitude
			Vector2 frictionImpulse;
			if (std::abs(jt) < j * staticFriction)
				frictionImpulse = tangent * jt; // static friction
			else
				frictionImpulse = tangent * -j * dynamicFriction; // dynamic friction

			// Apply the friction impulse to the bodies
			A->Impulse(-frictionImpulse, radA);
			B->Impulse(frictionImpulse, radB);
		}
	}

	void Manifold::PositionalCorrection()
	{
		const real k_slop = 0.05f; // Penetration allowance - usually 0.01 to 0.1 is good
		const real percent = 0.4f; // Penetration percentage to correct - usually 20% to 80%

		// Calculate the correction vector
		Vector2 correction = normal * (std::max(penetration - k_slop, 0.0f) / (A->InverseMass + B->InverseMass)) * percent;
		
		// Apply the correction to both bodies, scaled by their inverse mass
		A->position -= correction * A->InverseMass;
		B->position += correction * B->InverseMass;
	}

	void Manifold::InfiniteMassCorrection()
	{
		// For objects with infinite mass (immovable objects),
		// we simply set their velocities to zero
		A->velocity.Set(0.0f, 0.0f);
		B->velocity.Set(0.0f, 0.0f);
	}
}