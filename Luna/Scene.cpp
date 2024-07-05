#include "Libs.h"
#include "Material.h" 

namespace Luna
{
	Scene::Scene()
	{
		deltaTime = 0.0f;
		iterations = 0; 
		surfaceRB = nullptr;  
		AddMaterials(); // add materials to menu 
	}

	Scene::Scene(real deltaTime, size_t iterations) : deltaTime(deltaTime), iterations(iterations)
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
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* rbA = bodies[i];
			for (size_t j = i + 1; j < bodies.size(); j++)
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
		for (size_t i = 0; i < bodies.size(); i++)
			IntegradeForces(bodies[i], deltaTime);

		// initialize collision
		for (size_t i = 0; i < contacts.size(); i++)
			contacts[i].Init();

		// solve collisions
		for (size_t j = 0; j < iterations; j++)
			for (size_t i = 0; i < contacts.size(); i++)
				contacts[i].ApplyImpulse();

		// integrate velocity
		for (size_t i = 0; i < bodies.size(); i++)
		{
			
			RigidBody* b = bodies[i];
			IntegradeVelocity(b, deltaTime); 
			

			real brad = b->Radius;
			bool outOfBounds = (b->position.x + brad < -10 || b->position.x - brad > 90 ||
				b->position.y + brad < -10 || b->position.y - brad > 70);

			if (outOfBounds)
			{
				delete b;
				bodies.erase(bodies.begin() + i);
			}
		}

		// correct positions
		for (size_t i = 0; i < contacts.size(); i++)
			contacts[i].PositionalCorrection();

		// clear all forces
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->force.Set(0, 0);
			body->Torque = 0;
		}
	}

	void Scene::Render()
	{
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->shape->Draw(body);
		}
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			// Change color for contact points
			glColor4f(1.0f, 0.0f, 0.0f, 0.8f); // Red color for contact points
			for (size_t i = 0; i < contacts.size(); i++)
			{
				Manifold& m = contacts[i];
				for (size_t j = 0; j < m.contactCount; j++)
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
			for (size_t i = 0; i < contacts.size(); i++)
			{
				Manifold& m = contacts[i];
				Vector2 v = m.normal;

				for (size_t j = 0; j < m.contactCount; j++)
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

	RigidBody* Scene::Add(Shape* shape, size_t x, size_t y)
	{
		assert(shape);
		RigidBody* rb = new RigidBody(shape, x, y);
		rb->shape->rb = shape->rb;
		bodies.push_back(rb);
		return rb;
	}

	RigidBody* Scene::Add(Shape* shape, size_t x, size_t y, size_t i)
	{
		assert(shape);
		RigidBody* rb = new RigidBody(shape, x, y, objectMaterials[i]); 
		rb->shape->rb = shape->rb;
		bodies.push_back(rb);
		return rb;
	}

	void Scene::StrOnScene(int x, int y, const char* s)
	{
		glColor3f(0.5f, 0.5f, 0.9f);
		glRasterPos2i(x, y);
		size_t l = (size_t)std::strlen(s);
		for (size_t i = 0; i < l; ++i)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
	}

	void Scene::DrawMenu(bool menu, bool custom, bool surface, size_t selectedMaterialIndex, real* restitution, real* dynamicFriction, real* staticFriction)
	{
		if (!menu) return;

		glColor3f(0.2f, 0.2f, 0.2f);
		glRectf(10, 3, 70, 50);

		glColor3f(1.0f, 1.0f, 1.0f);
		StrOnScene(33, 6, "Settings Menu");
		StrOnScene(12, 8, "Press 'M' to close menu");
		StrOnScene(12, 10, "Press 'C' to go to deufault settings, Press 'U' to change surface"); 
		if (!surface) 
		{
			if (custom)
			{
				StrOnScene(12, 12, "[Custom]");
				StrOnScene(12, 14, "Press 'T' to switch to Predefined");
				StrOnScene(12, 35, "R: Restitution");
				StrOnScene(12, 40, "D: Dynamic Friction");
				StrOnScene(12, 45, "S: Static Friction");

				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", *restitution);
				StrOnScene(40, 35, buffer);
				sprintf_s(buffer, "Dyn. Friction: %.2f", *dynamicFriction);
				StrOnScene(40, 40, buffer);
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
				for (int i = 0; i < materialCountObject; i++)
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
				StrOnScene(12, 35, buffer);
				sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
				*dynamicFriction = selectedMaterial->getDynamicFriction(); 
				StrOnScene(12, 40, buffer);
				sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
				*staticFriction = selectedMaterial->getStaticFriction();  
				StrOnScene(12, 45, buffer);
			}
		}
		else
		{
				StrOnScene(12, 12, "Press 'N' for next and 'P' to go back"); 
				glColor3f(1.0f, 1.0f, 1.0f); 
				StrOnScene(15, 20, "<- "); 
				for (int i = 0; i < materialCountSurface; i++) 
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
				StrOnScene(12, 35, buffer);
				sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
				surfaceRB->SetDynamicFriction(selectedMaterial->getDynamicFriction());
				StrOnScene(12, 40, buffer);
				sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
				surfaceRB->SetStaticFriction(selectedMaterial->getStaticFriction());
				StrOnScene(12, 45, buffer);
		}
	}

	unsigned char* Scene::LoadBMP(const char* filename, int& width, int& height)
	{
		FILE* f = nullptr;
		errno_t err = fopen_s(&f, filename, "rb");
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
		ShapeCircle circle(4.0f);
		RigidBody* rb = Add(&circle, 20, 30);
		Material* material;

		material = surfaceMaterials[2];
		rb->SetMaterial(material);
		rb->SetStatic();
		rb->SetOrient(PI);
		std::cout << "Circle: Restitution: " << rb->GetRestitution() << " Dynamic Friction: " << rb->GetDynamicFriction() << " Static Friction: " << rb->GetStaticFriction() << std::endl; 

		ShapePolygon poly;
		poly.SetBox(30.0f, 1.0f);
		surfaceRB = Add(&poly, 40, 55);  
		material = surfaceMaterials[0]; 
		surfaceRB->SetMaterial(material);
		surfaceRB->SetStatic();
		surfaceRB->SetOrient(0);
		std::cout << "Poly: Restitution: " << surfaceRB->GetRestitution() << " Dynamic Friction: " << surfaceRB->GetDynamicFriction() << " Static Friction: " << surfaceRB->GetStaticFriction() << std::endl;

		ShapePolygon diagPoly;
		diagPoly.SetBox(10.0f, 1.0f);
		RigidBody* rbDiag = Add(&diagPoly, 70, 30);
		material = surfaceMaterials[1];
		rbDiag->SetMaterial(material);
		rbDiag->SetStatic(); 
		rbDiag->SetOrient(-PI / 6);
		std::cout << "DiagPoly: Restitution: " << rbDiag->GetRestitution() << " Dynamic Friction: " << rbDiag->GetDynamicFriction() << " Static Friction: " << rbDiag->GetStaticFriction() << std::endl;  
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
		size_t i = 3; 
		while (bodies.size() > i)
		{
			delete bodies[i];
			bodies.erase(bodies.begin() + i); 
		}
		 
		while (contacts.size() > 0)  
		{
			contacts.erase(contacts.begin() + 0);
		}
	}

	void Scene::AddMaterialObject(const std::string& name, real restitution, real dynamicFriction, real staticFriction)
	{
		Material* material = new Material(name, restitution, dynamicFriction, staticFriction); 
		objectMaterials.push_back(material);   
		materialCountObject = objectMaterials.size(); 
	}

	void Scene::AddMaterialSurface(const std::string& name, real restitution, real dynamicFriction, real staticFriction)
	{
		Material* material = new Material(name, restitution, dynamicFriction, staticFriction);
		surfaceMaterials.push_back(material);   
		materialCountSurface = surfaceMaterials.size();
	}


	// Mouse callback function for handling mouse clicks
	void Scene::mouse(int button, int state, int x, int y)
	{
		if (Menu || showLogo) return; // If the menu is open or logo is showing, don't add objects 

		// Convert the screen coordinates to simulation coordinates
		real x_ = x / 10.0f;
		real y_ = y / 10.0f;

		if (state == GLUT_DOWN)
		{
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
			{
				// Create and add a random polygon
				ShapePolygon poly;
				size_t count = (size_t)Random(3, MaxPolyCount);
				Vector2* vectors = new Vector2[count];
				real radius = Random(5, 10);
				for (size_t i = 0; i < count; i++)
					vectors[i].Set(Random(-radius, radius), Random(-radius, radius));
				poly.Set(vectors, count);
				RigidBody* rb = Add(&poly, (size_t)x_, (size_t)y_, selectedMaterialIndexObject);
				rb->SetOrient(Random(-PI, PI));
				std::cout << "Poly: Restitution: "
					<< rb->GetRestitution() << " Dynamic Friction: "
					<< rb->GetDynamicFriction() << " Static Friction: "
					<< rb->GetStaticFriction() << std::endl;
				delete[] vectors;
			}
			break;
			case GLUT_RIGHT_BUTTON:
			{
				// Create and add a random circle
				ShapeCircle circle(Random(1.0f, 5.0f));
				RigidBody* rb = Add(&circle, (size_t)x_, (size_t)y_, selectedMaterialIndexObject);
				std::cout << "Circle: Restitution: "
					<< rb->GetRestitution() << " Dynamic Friction: "
					<< rb->GetDynamicFriction() << " Static Friction: "
					<< rb->GetStaticFriction() << std::endl;
			}
			break;
			}
		}
	}

	void Scene::LunaLoop()
	{
		// Set up the OpenGL clear color and clear the screen and depth buffer
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up the orthogonal projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 80, 60, 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Render instructional text on the screen
		StrOnScene(1, 2, "Press 'Esc' to exit");
		StrOnScene(1, 4, "Left click add a polygon");
		StrOnScene(1, 6, "Right click add a circle");
		StrOnScene(1, 8, "Press 'M' to open menu");
		StrOnScene(1, 10, "Press 'S' to start/stop the simulation, 'F' to frame by frame");
		StrOnScene(1, 12, "Press 'C' to clear the scene");

		static real accumulator = 0.0f;

		accumulator += clock_time.Elapsed(); // Get the elapsed time since the last frame

		clock_time.Start(); // Restart the clock 

		// acmulator is the time that has passed since the last frame
		accumulator = Clamp(accumulator, 0.0f, 0.1f); // Clamp the accumulator to prevent spiral of death

		// If the accumulator is greater than the time step, step the scene
		while (accumulator >= deltaTime) 
		{
			if (!frameStep)
				Step();  
			else
				if (step)
				{
					Step(); 
					step = false;
				}

			accumulator -= deltaTime;
		}

		clock_time.Stop(); // Stop the clock

		Render(); // Render the scene 

		if (!ChangeSurface)
			DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexObject, &restitution, &dynamicFriction, &staticFriction);
		else
			DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexSurface, &restitution, &dynamicFriction, &staticFriction);

		glutSwapBuffers(); // Swap the buffers
	}

	// Main loop for the program
	void Scene::mainLoop()
	{
		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (showLogo)
			/*scene.*/DisplayLogo(); // Display the logo 
		else
			LunaLoop(); // Display the simulation
	}

	void Scene::keyboardArrowKeys(int key, int x, int y)
	{
		if (Menu)
		{
			switch (key)
			{
			case GLUT_KEY_RIGHT:
			{
				if (!ChangeSurface)
				{
					if (!custom && !ChangeSurface)
						selectedMaterialIndexObject = (selectedMaterialIndexObject + 1) % GetmaterialCountObject();
				}
				else
				{
					custom = false;
					selectedMaterialIndexSurface = (selectedMaterialIndexSurface + 1) % GetmaterialCountSurface();
				}
			}
			break;
			case GLUT_KEY_LEFT:
			{
				if (!custom && !ChangeSurface)
				{
					selectedMaterialIndexObject = (selectedMaterialIndexObject - 1) % GetmaterialCountObject();
					if (selectedMaterialIndexObject < 0)
						selectedMaterialIndexObject = GetmaterialCountObject() - 1;
				}
				else if (ChangeSurface)
				{
					custom = false;
					selectedMaterialIndexSurface = (selectedMaterialIndexSurface - 1) % GetmaterialCountSurface();
					if (selectedMaterialIndexSurface < 0)
						selectedMaterialIndexSurface = GetmaterialCountSurface() - 1;
				}
				break;
			}
			}
		}
	}

	// Keyboard callback function for menu interactions
	void Scene::KeyboardMenu(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 'r':
		case 'R':// change restitution
		{
			if (custom && !ChangeSurface)
			{
				restitution += 0.05f;
				if (restitution > 1.01f)
					restitution = 0.05f;
			}
		}
		break;
		case 'd':
		case 'D': // change dynamic friction
		{
			if (custom && !ChangeSurface)
			{
				dynamicFriction += 0.05f;
				if (dynamicFriction > 1.01f)
					dynamicFriction = 0.05f;
			}
		}
		break;
		case 'u':
		case 'U': // toggle between object and surface material
		{
			ChangeSurface = !ChangeSurface;
		}
		break;
		case 's':
		case 'S': // change static friction
		{
			if (custom && !ChangeSurface)
			{
				staticFriction += 0.05f;
				if (staticFriction > 1.01f)
					staticFriction = 0.05f;
			}
		}
		break;
		case 'c':
		case 'C': // reset values to default
		{
			if (!ChangeSurface)
			{
				restitution = 0.2f;
				dynamicFriction = 0.3f;
				staticFriction = 0.5f;
				selectedMaterialIndexObject = 0;
			}
			else
			{
				selectedMaterialIndexSurface = 0;
			}
		}
		break;
		case 't':
		case 'T': // toggle custom values
		{
			if (!ChangeSurface)
				custom = !custom;
		}
		break;
		//case 'n':
		//case 'N': // Select next material
		//{
		//	if (!ChangeSurface)
		//	{
		//		if (!custom && !ChangeSurface)
		//			selectedMaterialIndexObject = (selectedMaterialIndexObject + 1) % GetmaterialCountObject();
		//	}
		//	else
		//	{
		//		custom = false;
		//		selectedMaterialIndexSurface = (selectedMaterialIndexSurface + 1) % GetmaterialCountSurface();
		//	}
		//}
		//break;
		//case 'p':
		//case 'P': // Select previous material
		//{
		//	if (!custom && !ChangeSurface)
		//	{
		//		selectedMaterialIndexObject = (selectedMaterialIndexObject - 1) % GetmaterialCountObject();
		//		if (selectedMaterialIndexObject < 0)
		//			selectedMaterialIndexObject = GetmaterialCountObject() - 1;
		//	}
		//	else if (ChangeSurface)
		//	{
		//		custom = false;
		//		selectedMaterialIndexSurface = (selectedMaterialIndexSurface - 1) % GetmaterialCountSurface();
		//		if (selectedMaterialIndexSurface < 0)
		//			selectedMaterialIndexSurface = GetmaterialCountSurface() - 1;
		//	}
		//}
		break;
		case 'm':
		case 'M': // Close the menu
		{
			Menu = false;
			if (!Menu)
				frameStep = false;
		}
		default:
		{
			if (key != 'm' && key != 'M')
				std::cout << "Wrong key pressed..." << std::endl;
		}
		break;
		}
	}

	// General keyboard callback function 
	void Scene::keyboard(unsigned char key, int x, int y)
	{
		if (Menu)
			KeyboardMenu(key, x, y);
		else
		{
			switch (key)
			{
			case ESC: // Exit the program
				exit(0);
				break;
			case 'm':
			case 'M': // Open/Close the menu
				if (!showLogo)
				{
					Menu = true;
					frameStep = true;
				}
				break;
			case 'c':
			case 'C': // Clear the scene
				if (!showLogo)
					Clear();
				break;
			//case ' ': // Enter Simulation from Intro Logo Screen
			//	if (showLogo)
			//	{
			//		showLogo = false;
			//		glutDisplayFunc(LunaLoop);
			//		glutPostRedisplay();
			//	}
			//	break;
			case 's':
			case 'S': // Start/Stop the simulation
				if (!showLogo)
					frameStep = frameStep ? false : true;
				break;
			case 'f':
			case 'F': // Step through the simulation frame by frame
				if (!showLogo)
					step = true;
				break;
			default:
				std::cout << "Wrong key pressed..." << std::endl;
				break;
			}
		}
	}
}