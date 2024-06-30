#ifndef MANIFOLD_H
#define MANIFOLD_H

namespace Luna
{
	class RigidBody;
	class Manifold // Manifold is a class that holds the information of the collision between two bodies
	{
	public:
		RigidBody* A;
		RigidBody* B;
		real penetration;
		Vector2 normal;
		Vector2 contacts[2]; 
		size_t contactCount;

		real e;
		real dynamicFriction;;
		real staticFriction;

		Manifold();
		Manifold(RigidBody* a, RigidBody* b);
		~Manifold();

		void Solve();
		void Init();
		void ApplyImpulse();
		void PositionalCorrection();
		void InfiniteMassCorrection();
	};
}

#endif // !MANIFOLD_H
