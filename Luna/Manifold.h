#ifndef MANIFOLD_H
#define MANIFOLD_H

namespace Luna
{
	class RigidBody;
	class Manifold // Manifold is a class that holds the information of the collision between two bodies
	{
	public:
		RigidBody* A; // The two bodies that are colliding
		RigidBody* B; // The two bodies that are colliding
		real penetration; // The penetration depth of the collision
		Vector2 normal; // The normal of the collision
		Vector2 contacts[2];  // The contact points of the collision
		size_t contactCount; // The number of contact points

		real e; // The restitution of the collision
		real dynamicFriction;; // The dynamic friction of the collision
		real staticFriction; // The static friction of the collision

		Manifold(); // Default constructor
		Manifold(RigidBody* a, RigidBody* b); // Constructor that takes in two bodies
		~Manifold(); // Destructor

		void Solve(); // Solve the collision
		void Init(); // Initialize the collision
		void ApplyImpulse(); // Apply the impulse to the bodies
		void PositionalCorrection(); // Correct the position of the bodies
		void InfiniteMassCorrection(); // Correct the position of the bodies if one of them has infinite mass
	};
}

#endif // !MANIFOLD_H