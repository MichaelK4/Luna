#ifndef  SCENE_H
#define SCENE_H
//#include "Shape.h"
#include "Mat2.h"

namespace Luna
{
	class Scene
	{
	private:
		real deltaTime;
		uint iterations;
		std::vector<RigidBody*> bodies;
		std::vector<Manifold> contacts;

	public:
		Scene();
		Scene(real deltaTime, uint iterations);
		~Scene();

		void Step();
		void Render();
		RigidBody* Add(Shape* shape, uint x, uint y);
		SoftBody* AddSoft(Shape* shape, uint x, uint y);
		void Clear();
	};
}

#endif // ! SCENE_H



