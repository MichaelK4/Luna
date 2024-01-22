#pragma once
#include "RigidBody.h"
#include "Collider.h"

class Engine
{
public:

	Engine();
	~Engine();

	void Start();
	void Update(float deltaTime);
	void Render();

	void applyGravity(RigidBody& rigidBody, float deltaTime, float gravity);

	void resolveCollision(RigidBody& rigidBody, Collider& collider);

	void updateRigidBody(RigidBody& rigidBody, float deltaTime);

};

