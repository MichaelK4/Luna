//#ifndef SOFTBODY_H
//#define SOFTBODY_H
//
#include "RigidBody.h"
//
//
namespace Luna
{
	class SoftBody : public RigidBody
	{
	public:
		SoftBody();
		SoftBody(Shape* shape, size_t x, size_t y);
		~SoftBody();

		real Softness;
		real Damping;

		void Update(float deltaTime) override;
		//void Draw() override;
	};
}
//
//
//#endif // !SOFTBODY_H

