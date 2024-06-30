#include "Libs.h"
#include "Material.h" 

//int rb_num = 0;
namespace Luna
{
	Scene::Scene()
	{
		deltaTime = 0.0f;
		iterations = 0; 
		surfaceRB = nullptr;  
		AddMaterials(); // add materials to menu 
	}

	Scene::Scene(real deltaTime, uint iterations) : deltaTime(deltaTime), iterations(iterations) 
	{ 
		surfaceRB = nullptr; 
		AddMaterials(); // add materials to menu 
	}

	Scene::~Scene() 
	{
		if(surfaceRB)
		{
			delete surfaceRB; 
		}
	}

	void Scene::AddMaterials() 
	{
		// object materials
		AddMaterialObject("Default - Like Leather/Soft Plastic", 0.2f, 0.3f, 0.5f); // default 0.2 0.3 0.5 is like leather or soft plastic 
		AddMaterialObject("Steel", 0.7f, 0.3f, 0.4f); // steel
		AddMaterialObject("Rubber", 0.85f, 0.8f, 0.9f); // rubber   
		AddMaterialObject("Glass", 0.95f, 0.15f, 0.2f); // glass  
		AddMaterialObject("Plastic", 0.7f, 0.3f, 0.35f); // plastic  
		AddMaterialObject("Rock", 0.2f, 0.55f, 0.65f); // rock   
		AddMaterialObject("Concrete", 0.3f, 0.6f, 0.7f); // concrete  
		AddMaterialObject("Foam", 0.1f, 0.5f, 0.6f); // foam  
		AddMaterialObject("Leather", 0.4f, 0.55f, 0.65f); // sand 

		// surface materials
		AddMaterialSurface("Default - Wood", 0.4f, 0.35f, 0.45f); // default  
		AddMaterialSurface("Ice", 0.01f, 0.02f, 0.03f); // ice  
		AddMaterialSurface("Concrete", 0.30f, 0.60f, 0.70f); // concrete
		AddMaterialSurface("Rock", 0.2f, 0.55f, 0.65f); // rock
		AddMaterialSurface("Sponge", 0.20f, 0.70f, 0.75f); // sponge
		AddMaterialSurface("Carpet", 0.30f, 0.80f, 0.85f); // carpet
		AddMaterialSurface("Teflon", 0.05f, 0.04f, 0.06f); // teflon
		AddMaterialSurface("Mud", 0.05f, 0.95f, 0.99f); // mud
		AddMaterialSurface("Trampoline", 0.90f, 0.25f, 0.30f); // trampoline
	}

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
					contacts.emplace_back(m);
			}
		}

		// integrate forces
		for (uint i = 0; i < bodies.size(); i++)
			IntegradeForces(bodies[i], deltaTime);

		// initialize collision
		for (uint i = 0; i < contacts.size(); i++)
			contacts[i].Init();

		// solve collisions
		for (uint j = 0; j < iterations; j++)
			for (uint i = 0; i < contacts.size(); i++)
				contacts[i].ApplyImpulse();

		// integrate velocity
		for (uint i = 0; i < bodies.size(); i++)
		{
			
			RigidBody* b = bodies[i];
			IntegradeVelocity(b, deltaTime); 
			

			real brad = b->Radius;
			bool outOfBounds = (b->position.x + brad < -10 || b->position.x - brad > 90 ||
				b->position.y + brad < -10 || b->position.y - brad > 70);

			if (outOfBounds)
			{
				// DEBUGGING REASONS
				/* std::cout << "Body num:" << rb_num << " at position" << "(" << b->position.x << ", " << b->position.y
					<< ") with radius " << brad << " is out of bounds." << std::endl; */
				delete b;
				bodies.erase(bodies.begin() + i);
			}
			// DEBUGGING REASONS
			/*else
			{
				if (!b->isStatic())
				{
					
					std::cout << "Body num:" << rb_num << " at position" << "(" << b->position.x << ", " << b->position.y
						<< ") with radius " << brad << " is out of bounds But don't dissaper" << std::endl;
					b->staticB = true; 
				}
			}*/
		}

		// correct positions
		for (uint i = 0; i < contacts.size(); i++)
			contacts[i].PositionalCorrection();

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
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			// Change color for contact points
			glColor4f(1.0f, 0.0f, 0.0f, 0.8f); // Red color for contact points
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

		glLineWidth(4.0f);
		glBegin(GL_LINES);
			// Change color for collision normals
			glColor4f(0.0f, 0.0f, 1.0f, 0.8f); // Blue color for collision normals
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
		glLineWidth(1.0f);
	}

	RigidBody* Scene::Add(Shape* shape, uint x, uint y)
	{
		//rb_num++;
		assert(shape);
		RigidBody* rb = new RigidBody(shape, x, y);
		rb->shape->rb = shape->rb;
		bodies.push_back(rb);
		return rb;
	}

	RigidBody* Scene::Add(Shape* shape, uint x, uint y, size_t i)
	{
		assert(shape);
		RigidBody* rb = new RigidBody(shape, x, y, objectMaterials[i]); 
		rb->shape->rb = shape->rb;
		bodies.push_back(rb);
		return rb;
	}

	/*SoftBody* Scene::AddSoft(Shape* shape, uint x, uint y)
	{
		assert(shape);
		SoftBody* sb = new SoftBody(shape, x, y);
		sb->shape->rb = shape->rb;
		bodies.push_back(sb);
		return sb;
	}*/


	void Scene::StrOnScene(int x, int y, const char* s)
	{
		glColor3f(0.5f, 0.5f, 0.9f);
		glRasterPos2i(x, y);
		size_t l = (size_t)std::strlen(s);
		for (size_t i = 0; i < l; ++i)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
	}


	void Scene::DrawMenu(bool menu, bool custom, bool surface, int selectedMaterialIndex, real* restitution, real* dynamicFriction, real* staticFriction)  
	{
		if (!menu) return;


		glColor3f(0.2f, 0.2f, 0.2f);
		glRectf(10, 3, 70, 50);

		glColor3f(1.0f, 1.0f, 1.0f);
		StrOnScene(12, 6, "Settings Menu");
		StrOnScene(12, 8, "Press 'M' to close menu");
		StrOnScene(12, 10, "Press C to go to deufault settings, Press U to change surface"); 
		if (!surface) 
		{
			if (custom)
			{
				StrOnScene(12, 12, "[Custom]");
				StrOnScene(12, 14, "Press 'T' to switch to Predefined");
				StrOnScene(12, 25, "R: Restitution");
				StrOnScene(12, 35, "D: Dynamic Friction");
				StrOnScene(12, 45, "S: Static Friction");

				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", *restitution);
				StrOnScene(40, 25, buffer);
				sprintf_s(buffer, "Dyn. Friction: %.2f", *dynamicFriction);
				StrOnScene(40, 35, buffer);
				sprintf_s(buffer, "Stat. Friction: %.2f", *staticFriction);
				StrOnScene(40, 45, buffer);
			}
			else
			{
				StrOnScene(12, 12, "[Predefined]");
				StrOnScene(12, 14, "Press 'T' to switch to Custom");
				StrOnScene(12, 16, "Press 'N' for next and 'P' to go back");
				glColor3f(1.0f, 1.0f, 1.0f);
				StrOnScene(15, 20, "<- ");
				for (size_t i = 0; i < materialCountObject; i++)
				{
					if (i == selectedMaterialIndex)
					{
						glColor3f(0.0f, 1.0f, 0.0f);
						StrOnScene(18, 20, objectMaterials[i]->getName().c_str());    
						StrOnScene(18 + objectMaterials[i]->getName().size(), 20, " ->"); 
					}
				}

				const Material* selectedMaterial = objectMaterials[selectedMaterialIndex]; 
				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", selectedMaterial->getRestitution());
				*restitution = selectedMaterial->getRestitution();  
				StrOnScene(12, 25, buffer);
				sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
				*dynamicFriction = selectedMaterial->getDynamicFriction(); 
				StrOnScene(12, 35, buffer);
				sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
				*staticFriction = selectedMaterial->getStaticFriction();  
				StrOnScene(12, 45, buffer);
			}
		}
		else
		{
				StrOnScene(12, 16, "Press 'N' for next and 'P' to go back"); 
				glColor3f(1.0f, 1.0f, 1.0f); 
				StrOnScene(15, 20, "<- "); 
				for (size_t i = 0; i < materialCountSurface; i++) 
				{
					if (i == selectedMaterialIndex) 
					{
						glColor3f(0.0f, 1.0f, 0.0f); 
						StrOnScene(18, 20, surfaceMaterials[i]->getName().c_str());  
						StrOnScene(18 + surfaceMaterials[i]->getName().size(), 20, " ->"); 
					}
				}

				const Material* selectedMaterial = surfaceMaterials[selectedMaterialIndex]; 
				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", selectedMaterial->getRestitution());
				surfaceRB->SetRestitution(selectedMaterial->getRestitution()); 
				StrOnScene(12, 25, buffer);
				sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
				surfaceRB->SetDynamicFriction(selectedMaterial->getDynamicFriction());
				StrOnScene(12, 35, buffer);
				sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
				surfaceRB->SetStaticFriction(selectedMaterial->getStaticFriction());
				StrOnScene(12, 45, buffer);
		}
	}

	unsigned char* Scene::LoadBMP(const char* filename, int& width, int& height)
	{
		FILE* f = nullptr;
		errno_t err = fopen_s(&f, filename, "rb");
		//FILE fp = fopen_s(&fp, filename, "rb");
		if (!f)
		{
			std::cerr << "Error opening the file" << std::endl;
			return nullptr;
		}
		unsigned char header[54]; // BMP header is 54 bytes
		if (fread(header, 1, 54, f) != 54) {
			std::cerr << "Not a correct BMP file: " << filename << std::endl;
			fclose(f);
			return nullptr;
		}
		width = *(int*)&(header[18]);
		height = *(int*)&(header[22]);
		int imageSize = width * height * 3; // 3 bytes per pixel for RGB

		unsigned char* data = new unsigned char[imageSize];
		fread(data, 1, imageSize, f);
		fclose(f);

		return data;
	}

	void Scene::LoadLogoTexture(const char* filename)
	{
		int width, height;
		unsigned char* image = LoadBMP(filename, width, height);
		if (!image)
		{
			std::cerr << "Error loading the image" << std::endl;
			exit(1);
		}
		else
		{
			GLuint tmp = GetLogoTexture();
			glGenTextures(1, &tmp); 
			glBindTexture(GL_TEXTURE_2D, GetLogoTexture());

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			delete[] image;
		}
	}

	void Scene::DisplayLogo()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, HEIGHT, 0); // Set orthogonal projection to match window dimensions

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//StrOnScene(10, 5, "Press 'Space' to start engine");

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, GetLogoTexture());

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);     // Bottom-left
			glTexCoord2f(1.0f, 1.0f); glVertex2f((real)WIDTH, 0.0f);    // Bottom-right
			glTexCoord2f(1.0f, 0.0f); glVertex2f((real)WIDTH, (real)HEIGHT);  // Top-right
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, (real)HEIGHT);   // Top-left
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glutSwapBuffers();
	}

	void Scene::ShowOnDisplay()
	{
		Circle circle(4.0f);
		RigidBody* rb = Add(&circle, 20, 30);
		Material* material;

		material = surfaceMaterials[2];
		rb->SetMaterial(material);
		//rb->SetRestitution(0.1f);  
		//rb->SetDynamicFriction(0.7f);  
		//rb->SetStaticFriction(0.9f);  
		rb->SetStatic();
		rb->SetOrient(PI);
		std::cout << "Circle: Restitution: " << rb->GetRestitution() << " Dynamic Friction: " << rb->GetDynamicFriction() << " Static Friction: " << rb->GetStaticFriction() << std::endl; 

		ShapePolygon poly;
		poly.SetBox(30.0f, 1.0f);
		surfaceRB = Add(&poly, 40, 55);  
		material = surfaceMaterials[0]; 
		surfaceRB->SetMaterial(material);
		//surfaceRB->SetRestitution(0.3f);
		//surfaceRB->SetDynamicFriction(0.4f);
		//surfaceRB->SetStaticFriction(0.5f); 
		surfaceRB->SetStatic();
		surfaceRB->SetOrient(0);
		std::cout << "Poly: Restitution: " << surfaceRB->GetRestitution() << " Dynamic Friction: " << surfaceRB->GetDynamicFriction() << " Static Friction: " << surfaceRB->GetStaticFriction() << std::endl;

		ShapePolygon diagPoly;
		//Vector2 diagVec[4] = { Vector2(-1, -1),Vector2(1, -1),
									//Vector2(8, 8), Vector2(-8, 8) };
		//diagPoly.Set(diagVec, 4);
		diagPoly.SetBox(10.0f, 1.0f);
		RigidBody* rbDiag = Add(&diagPoly, 70, 30);
		material = surfaceMaterials[1];
		rbDiag->SetMaterial(material);
		//rbDiag->SetRestitution(0.05f); 
		//rbDiag->SetDynamicFriction(0.05f); 
		//rbDiag->SetStaticFriction(0.1f); 
		rbDiag->SetStatic(); 
		rbDiag->SetOrient(-PI / 6);
		std::cout << "diagPoly: Restitution: " << rbDiag->GetRestitution() << " Dynamic Friction: " << rbDiag->GetDynamicFriction() << " Static Friction: " << rbDiag->GetStaticFriction() << std::endl;  

	}

	void Scene::Init()
	{
		// Set up OpenGL
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}

	void Scene::Clear()
	{
		int i = 3;
		while (bodies.size() > i)
		{
			delete bodies[i];
			bodies.erase(bodies.begin() + i); 
		}
	}

	void Scene::AddMaterialObject(const std::string& name, real restitution, real dynamicFriction, real staticFriction)
	{
		Material* material = new Material(name, restitution, dynamicFriction, staticFriction); 
		//material.name = name; 
		//material.restitution = restitution; 
		//material.dynamicFriction = dynamicFriction; 
		//material.staticFriction = staticFriction; 
		objectMaterials.push_back(material);   
		materialCountObject = objectMaterials.size(); 
	}

	void Scene::AddMaterialSurface(const std::string& name, real restitution, real dynamicFriction, real staticFriction)
	{
		Material* material = new Material(name, restitution, dynamicFriction, staticFriction);
		//material.name = name; 
		//material.restitution = restitution; 
		//material.dynamicFriction = dynamicFriction; 
		//material.staticFriction = staticFriction; 
		surfaceMaterials.push_back(material);   
		materialCountSurface = surfaceMaterials.size();
	}

}
