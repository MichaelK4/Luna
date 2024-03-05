#ifndef MANIFOLD_H
#define MANIFOLD_H

#include "RigidBody.h"
//#include "Collider.h"

namespace Luna
{
	class Manifold
	{
	public:
		RigidBody* A;
		RigidBody* B;
		real penetration;
		Vector2 normal;
		Vector2 contacts[2];
		uint contactCount;

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
