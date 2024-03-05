#ifndef  SCENE_H
#define SCENE_H
//#include "Shape.h"
#include "Manifold.h"

namespace Luna
{
	class Scene
	{
	private:
		float deltaTime;
		uint iterations;
		std::vector<RigidBody*> bodies;
		std::vector<Manifold> contacts;

	public:
		Scene();
		Scene(float deltaTime, uint iterations);
		~Scene();

		void Step();
		void Render();
		RigidBody* Add(Shape* shape, uint x, uint y);
		void Clear();
	};
}

#endif // ! SCENE_H



