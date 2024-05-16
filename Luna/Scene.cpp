#include "Libs.h"


namespace Luna
{
	Scene::Scene()
	{
		deltaTime = 0.0f;
		iterations = 0;
	}

	Scene::Scene(real deltaTime, uint iterations) : deltaTime(deltaTime), iterations(iterations)
	{ }

	Scene::~Scene() {}

	void IntegradeForces(RigidBody* body, real deltaTime)
	{
		if (body->InverseMass == 0.0f)
			return;

		body->velocity += (body->force * body->InverseMass + GRAVITY) * (deltaTime / 2.0f);
		body->AngularVelocity += body->Torque * body->InverseInertia * (deltaTime / 2.0f);
	}

	void IntegradeVelocity(RigidBody* body, real deltaTime)
	{
		if (body->InverseMass == 0.0f)
			return;

		body->position += body->velocity * deltaTime;
		body->SetOrient(body->Orient + body->AngularVelocity * deltaTime);
		IntegradeForces(body, deltaTime);
	}

	void Scene::Step()
	{
		// clear all contacts
		contacts.clear();

		// generate contacts
		for (uint i = 0; i < bodies.size(); i++)
		{
			RigidBody* rbA = bodies[i];

			for (uint j = i + 1; j < bodies.size(); j++)
			{
				RigidBody* rbB = bodies[j];
				if (rbA->InverseMass == 0 && rbB->InverseMass == 0)
					continue;
				Manifold m(rbA, rbB);
				m.Solve();
				if (m.contactCount)
				{
					contacts.emplace_back(m);
				}
			}
		}

		// integrate forces
		for (uint i = 0; i < bodies.size(); i++)
		{
			IntegradeForces(bodies[i], deltaTime);
		}

		// initialize collision
		for (uint i = 0; i < contacts.size(); i++)
		{
			contacts[i].Init();
		}

		// solve collisions
		for (uint j = 0; j < iterations; j++)
		{
			for (uint i = 0; i < contacts.size(); i++)
			{
				contacts[i].ApplyImpulse();
			}
		}

		// integrate velocity
		for (uint i = 0; i < bodies.size(); i++)
		{
			IntegradeVelocity(bodies[i], deltaTime);
		}

		// correct positions
		for (uint i = 0; i < contacts.size(); i++)
		{
			contacts[i].PositionalCorrection();
		}

		// clear all forces
		for (uint i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->force.Set(0, 0);
			body->Torque = 0;
		}
	}

	void Scene::Render()
	{
		for (uint i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->shape->Draw(body);
		}
		glPointSize(4.0f);
		glBegin(GL_POINTS);
		// Change color for contact points
		glColor3f(1.0f, 0.0f, 0.0f); // Red color for contact points
		for (uint i = 0; i < contacts.size(); i++)
		{
			Manifold& m = contacts[i];
			for (uint j = 0; j < m.contactCount; j++)
			{
				Vector2 c = m.contacts[j];
				glVertex2f(c.x, c.y);
			}
		}
		glEnd();
		glPointSize(1.0f);

		//glLineWidth(3.0f);
		glBegin(GL_LINES);
		// Change color for collision normals
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color for collision normals
		for (uint i = 0; i < contacts.size(); i++)
		{
			Manifold& m = contacts[i];
			Vector2 v = m.normal;
			for (uint j = 0; j < m.contactCount; j++)
			{
				Vector2 c = m.contacts[j];
				glVertex2f(c.x, c.y);
				v *= 0.75f;
				c += v;
				glVertex2f(c.x, c.y);
			}
		}
		glEnd();

		////glPointSize(4.0f);
		//glBegin(GL_POINTS);
		//glColor3f(1.0f, 0.0f, 0.0f);
		//for (uint i = 0; i < contacts.size(); i++)
		//{
		//	Manifold& m = contacts[i];
		//	for (uint j = 0; j < m.contactCount; j++)
		//	{
		//		Vector2 c = m.contacts[j];
		//		glVertex2f(c.x, c.y);
		//	}
		//}
		//glEnd();
		//glPointSize(1.0f);

		////glLineWidth(3.0f);
		//glBegin(GL_LINES);
		//glColor3f(0.0f, 1.0f, 0.0f);
		//for (uint i = 0; i < contacts.size(); i++)
		//{
		//	Manifold& m = contacts[i];
		//	Vector2 v = m.normal;
		//	for (uint j = 0; j < m.contactCount; j++)
		//	{
		//		Vector2 c = m.contacts[j];
		//		glVertex2f(c.x, c.y);
		//		v *= 0.75f;
		//		c += v;
		//		glVertex2f(c.x, c.y);
		//	}
		//}
		//glEnd();
	}

	RigidBody* Scene::Add(Shape* shape, uint x, uint y)
	{
		assert(shape);
		RigidBody* rb = new RigidBody(shape, x, y);
		rb->shape->rb = shape->rb;
		bodies.push_back(rb);
		return rb;
	}

	SoftBody* Scene::AddSoft(Shape* shape, uint x, uint y)
	{
		assert(shape);
		SoftBody* sb = new SoftBody(shape, x, y);
		sb->shape->rb = shape->rb;
		bodies.push_back(sb);
		return sb;
	}


}
