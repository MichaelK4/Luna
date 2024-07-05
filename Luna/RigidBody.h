#ifndef RIGID_BODY_H
#define RIGID_BODY_H
#include "Vector2.h"
//#include "Material.h"

namespace Luna
{
	struct Shape;
	struct Material; 

	class RigidBody
	{
	private:
		/*Static friction is the friction that exists
		between a stationary object and
		the surface on which it's resting.
		The higher the value, the more friction there is.*/
		// Ideally, static friction should be greater than dynamic friction. The best valuie is 0.5f
		real StaticFriction;
		/*Dynamic friction is the friction that exists
		between a moving object and
		the surface on which it's moving.
		The higher the value, the more friction there is.*/
		// Ideally, dynamic friction should be less than static friction. The best value is 0.3f
		real DynamicFriction;
		/*Restitution is the measure of how
		much an object bounces off a surface.
		The higher the value, the more bouncy the object is. */
		// Ideally, restitution should be between 0 and 1. The best value is 0.2f
		real Restitution;
	public:
		Shape* shape; // Shape is the form of an object or its external boundary, outline, or external surface, as opposed to other properties such as color, texture, or material composition.
		Material* material; // Material is the physical substance of which objects are made.
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

		RigidBody(); // Default constructor
		RigidBody(Shape* shape, size_t x, size_t y); // Constructor
		RigidBody(Shape* shape, size_t x, size_t y, Material* material); // Constructor
		~RigidBody(); // Destructor

		void Force(const Vector2& force); // Force is a push or pull on an object resulting from the object's interaction with another object.
		void Impulse(const Vector2& impulse, const Vector2& contactVector); // Impulse is the change in momentum of an object when a force is applied over time.
		void SetOrient(real radians); // Set the orientation of an object in a two-dimensional space at a particular time.
		void SetStatic(); // Set the object to be static.
		void SetStaticFriction(real sf); // Set the static friction of an object.
		void SetDynamicFriction(real df); // Set the dynamic friction of an object.
		void SetRestitution(real r);  // Set the restitution of an object.
		void SetMaterial(Material* m);  // Set the material of an object.

		real GetStaticFriction(); // Get the static friction of an object.
		real GetDynamicFriction(); // Get the dynamic friction of an object.
		real GetRestitution(); // Get the restitution of an object.
		Material* GetMaterial(); // Get the material of an object.
	};
}

#endif // !RIGID_BODY_H