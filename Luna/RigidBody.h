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

		//bool staticB = false; // DEBUGGING REASONS
		//int b_num; // DEBUGGING REASONS
		Shape* shape;
		Material* material; 
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

		RigidBody();
		RigidBody(Shape* shape, size_t x, size_t y);
		RigidBody(Shape* shape, size_t x, size_t y, Material* material);
		~RigidBody();

		void Force(const Vector2& force);
		void Impulse(const Vector2& impulse, const Vector2& contactVector);
		void SetOrient(real radians);
		void SetStatic();
		void SetStaticFriction(real sf);
		void SetDynamicFriction(real df);
		void SetRestitution(real r); 
		void SetMaterial(Material* m);  

		real GetStaticFriction(); 
		real GetDynamicFriction();
		real GetRestitution();
		Material* GetMaterial();  
		// FOR DEBUGGING
		//bool isStatic()
		//{
		//	return staticB;
		//}
	};
}

#endif // !RIGID_BODY_H
