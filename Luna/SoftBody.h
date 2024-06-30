#ifndef SOFT_BODY_H
#define SOFT_BODY_H
#include "Vector2.h"

namespace Luna
{
	struct Shape; 

	class SoftBody
	{
	public:
		Shape* shape;
		SoftBody();
		SoftBody(Shape* shape, size_t x, size_t y);
		~SoftBody();

		real Softness; // Softness is used to determine how much the particles will deform when they collide with each other
		real Plasticity; // Plasticity is used to determine how much the particles will deform when they collide with each other
		real Elasticity; // Elasticity is used to determine how much the particles will bounce off of each other
		real Friction; // Friction is used to reduce the velocity of the particles
		real Damping; // Damping factor is used to reduce the velocity of the particles

		void Update(float deltaTime);
	};
}
#endif // !SOFTBODY_H

