#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "Vector2.h"

namespace Luna
{
	struct Shape;

	class RigidBody
	{
	private:
		uint x;
		uint y;

	public:
		Shape* shape;
		real r, b, g;
		Vector2 position; // Position is the location of an object in a two-dimensional space at a particular time.
		Vector2 velocity; // Velocity is the rate of change of the position of an object, equivalent to a specification of its speed and direction of motion.
		Vector2 force; // Force is a push or pull on an object resulting from the object's interaction with another object.	

		real AngularVelocity; // Angular velocity is the rate of change of the orientation of an object with respect to time.
		real Torque; // Torque is the tendency of a force to rotate an object about an axis.
		real Orient; // Orient is the orientation of an object in a two-dimensional space at a particular time.


		real Radius; // Radius is the distance from the center of an object to its edge.
		real Inertia; // Inertia is the resistance of any physical object to any change in its velocity.
		real InverseInertia; // Inverse inertia is the reciprocal of the inertia.
		real Mass; // Mass is the amount of matter in an object.
		real InverseMass; // Inverse mass is the reciprocal of the mass.

		/*Static friction is the friction that exists
		between a stationary object and
		the surface on which it's resting.*/
		real StaticFriction;
		/*Dynamic friction is the friction that exists
		between a moving object and
		the surface on which it's moving.*/
		real DynamicFriction;
		/*Restitution is the measure of how
		much an object bounces off a surface.*/
		real Restitution;

		RigidBody();
		RigidBody(Shape* shape, size_t x, size_t y);
		~RigidBody();

		void Force(const Vector2& force);
		void Impulse(const Vector2& impulse, const Vector2& contactVector);
		void SetOrient(real radians);
		void SetStatic();



		// updtae position based on velocity
		void Update(float deltaTime);

	};
}


#endif // !RIGID_BODY_H
