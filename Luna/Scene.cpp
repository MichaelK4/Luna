#include "Libs.h"
#include "Material.h" 

namespace Luna
{
	Scene::Scene()
	{
		// Initialize default values for the scene
		deltaTime = 0.0f;
		iterations = 0; 
		surfaceRB = nullptr;  
		AddMaterials(); // Add predefined materials to the menu 
	}

	Scene::Scene(real deltaTime, size_t iterations) : deltaTime(deltaTime), iterations(iterations)
	{ 
		// Constructor with specified deltaTime and iterations
		surfaceRB = nullptr; 
		AddMaterials(); // Add predefined materials to the menu 
	}

	// Destructor: clean up dynamically allocated memory
	Scene::~Scene() { if(surfaceRB) delete surfaceRB; }

	void Scene::AddMaterials() 
	{
		// Add predefined materials for objects
		// Parameters: name, restitution, dynamic friction, static friction
		AddMaterialObject("Default - Steel", 0.7f, 0.3f, 0.4f); // steel
		AddMaterialObject("Rubber", 0.85f, 0.8f, 0.9f); // rubber   
		AddMaterialObject("Glass", 0.95f, 0.15f, 0.2f); // glass  
		AddMaterialObject("Plastic", 0.7f, 0.3f, 0.35f); // plastic  
		AddMaterialObject("Rock", 0.2f, 0.55f, 0.65f); // rock   
		AddMaterialObject("Concrete", 0.3f, 0.6f, 0.7f); // concrete  
		AddMaterialObject("Foam", 0.1f, 0.5f, 0.6f); // foam  
		AddMaterialObject("Leather", 0.4f, 0.55f, 0.65f); // leather 
		AddMaterialObject("Wood", 0.5f, 0.4f, 0.5f); // wood 
		AddMaterialObject("Aluminum", 0.6f, 0.25f, 0.35f); // aluminum 
		AddMaterialObject("Ceramic", 0.8f, 0.2f, 0.25f); // ceramic 
		AddMaterialObject("Fabric", 0.3f, 0.6f, 0.7f); // fabric 
		AddMaterialObject("Cork", 0.2f, 0.4f, 0.5f); // cork 
		AddMaterialObject("Titanium", 0.75f, 0.35f, 0.45f); // titanium
		AddMaterialObject("Copper", 0.65f, 0.28f, 0.38f); // copper
		AddMaterialObject("Brass", 0.68f, 0.32f, 0.42f); // brass
		AddMaterialObject("Silicone", 0.8f, 0.75f, 0.85f); // silicone
		AddMaterialObject("Nylon", 0.5f, 0.35f, 0.45f); // nylon
		AddMaterialObject("Porcelain", 0.85f, 0.18f, 0.23f); // porcelain
		AddMaterialObject("Carbon Fiber", 0.55f, 0.28f, 0.38f); // carbon fiber
		AddMaterialObject("Fiberglass", 0.6f, 0.3f, 0.4f); // fiberglass
		AddMaterialObject("Felt", 0.25f, 0.65f, 0.75f); // felt
		AddMaterialObject("Graphite", 0.4f, 0.2f, 0.3f); // graphite

		// Add predefined materials for surfaces
		AddMaterialSurface("Default - Wood", 0.4f, 0.35f, 0.45f); // default  
		AddMaterialSurface("Ice", 0.01f, 0.02f, 0.03f); // ice  
		AddMaterialSurface("Concrete", 0.30f, 0.60f, 0.70f); // concrete
		AddMaterialSurface("Rock", 0.2f, 0.55f, 0.65f); // rock
		AddMaterialSurface("Sponge", 0.20f, 0.70f, 0.75f); // sponge
		AddMaterialSurface("Carpet", 0.30f, 0.80f, 0.85f); // carpet
		AddMaterialSurface("Teflon", 0.05f, 0.04f, 0.06f); // teflon
		AddMaterialSurface("Mud", 0.05f, 0.95f, 0.99f); // mud
		AddMaterialSurface("Trampoline", 0.90f, 0.25f, 0.30f); // trampoline
		AddMaterialSurface("Grass", 0.35f, 0.7f, 0.8f); // grass 
		AddMaterialSurface("Sand", 0.25f, 0.65f, 0.75f); // sand 
		AddMaterialSurface("Metal Grate", 0.6f, 0.3f, 0.4f); // metal grate 
		AddMaterialSurface("Rubber Mat", 0.8f, 0.75f, 0.85f); // rubber mat 
		AddMaterialSurface("Polished Marble", 0.7f, 0.05f, 0.1f); // polished marble 
		AddMaterialSurface("Astroturf", 0.4f, 0.65f, 0.75f); // astroturf
		AddMaterialSurface("Linoleum", 0.5f, 0.2f, 0.3f);  // linoleum
		AddMaterialSurface("Cork Flooring", 0.3f, 0.45f, 0.55f); // cork flooring
		AddMaterialSurface("Rubber Playground", 0.75f, 0.7f, 0.8f); // rubber playground
		AddMaterialSurface("Gravel", 0.15f, 0.6f, 0.7f); // gravel
		AddMaterialSurface("Asphalt", 0.25f, 0.5f, 0.6f); // asphalt
		AddMaterialSurface("Laminate Flooring", 0.45f, 0.25f, 0.35f); // laminate flooring 
		AddMaterialSurface("Vinyl", 0.4f, 0.3f, 0.4f); // vinyl
		AddMaterialSurface("Tatami Mat", 0.35f, 0.75f, 0.85f); // tatami mat
		AddMaterialSurface("Brushed Steel", 0.65f, 0.2f, 0.3f); // brushed steel
	}

	void IntegradeForces(RigidBody* body, real deltaTime)
	{
		// Skip integration for static bodies (infinite mass)
		if (body->InverseMass == 0.0f) return;

		// Update velocity based on forces and gravity
		body->velocity += (body->force * body->InverseMass + GRAVITY) * (deltaTime / 2.0f);
		// Update angular velocity based on torque
		body->AngularVelocity += body->Torque * body->InverseInertia * (deltaTime / 2.0f);
	}

	void IntegradeVelocity(RigidBody* body, real deltaTime)
	{
		// Skip integration for static bodies (infinite mass)
		if (body->InverseMass == 0.0f) return;

		// Update position based on velocity
		body->position += body->velocity * deltaTime;
		// Update orientation based on angular velocity
		body->SetOrient(body->Orient + body->AngularVelocity * deltaTime);
		// Integrate forces again for improved accuracy (Verlet integration)
		IntegradeForces(body, deltaTime);
	}

	void Scene::Step()
	{
		// Clear previous frame's contacts
		contacts.clear();

		// Generate new contacts between bodies
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* rbA = bodies[i];
			for (size_t j = i + 1; j < bodies.size(); j++)
			{
				RigidBody* rbB = bodies[j];
				// Skip collision check between two static bodies
				if (rbA->InverseMass == 0 && rbB->InverseMass == 0) continue;
				// Create and solve manifold for potential collision
				Manifold m(rbA, rbB);
				m.Solve();
				if (m.contactCount)
					contacts.emplace_back(m);
			}
		}

		// Integrate forces for all bodies
		for (size_t i = 0; i < bodies.size(); i++) IntegradeForces(bodies[i], deltaTime);

		// Initialize collision resolution
		for (size_t i = 0; i < contacts.size(); i++) contacts[i].Init();

		// Iteratively solve collisions
		for (size_t j = 0; j < iterations; j++)
			for (size_t i = 0; i < contacts.size(); i++) 
				contacts[i].ApplyImpulse();

		// Integrate velocities and update positions
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* b = bodies[i];
			IntegradeVelocity(b, deltaTime); 
			
			// Check if body is out of bounds and remove if necessary 
			real r = b->Radius; 
			bool outOfBounds = (b->position.x + r < -10  ||
								b->position.x - r >  90  ||
								b->position.y + r < -100 ||
								b->position.y - r >  120); 
			if (outOfBounds)
			{
				delete b;
				bodies.erase(bodies.begin() + i);
			}
		}

		// Apply positional corrections to prevent sinking
		for (size_t i = 0; i < contacts.size(); i++) contacts[i].PositionalCorrection();

		// Clear forces for the next frame
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->force.Set(0, 0);
			body->Torque = 0;
		}
	}

	void Scene::Render()
	{
		// Render all rigid bodies
		for (size_t i = 0; i < bodies.size(); i++)
		{
			RigidBody* body = bodies[i];
			body->shape->Draw(body);
		}

		// Render contact points
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
		assert(shape); // Ensure the shape pointer is valid
		// Create a new RigidBody with the given shape and position
		RigidBody* rb = new RigidBody(shape, x, y); 
		rb->shape->rb = shape->rb; // Link the shape to the RigidBody
		bodies.push_back(rb); // Add the new RigidBody to the scene
		return rb;
	}

	RigidBody* Scene::Add(Shape* shape, size_t x, size_t y, size_t i)
	{ 
		assert(shape); // Ensure the shape pointer is valid
		// Create a new RigidBody with the given shape and position
		RigidBody* rb = new RigidBody(shape, x, y, objectMaterials[i]); 
		rb->shape->rb = shape->rb; // Link the shape to the RigidBody
		
		// Set the custom material flag
		if(custom && clickedCustomParameters) rb->setIsCustomMaterial(true);
		else rb->setIsCustomMaterial(false);

		bodies.push_back(rb); // Add the new RigidBody to the scene
		return rb;
	}

	void Scene::StrOnScene(int x, int y, const char* s, bool isSim) 
	{
		// Set the color based on whether it's simulation text or menu text
		if(!isSim) 	
			glColor3f(1.0f, 1.0f, 1.0f); // White color for menu text
		else
			glColor3f(0.0f, 0.0f, 0.0f); // Black color for simulation text
		glRasterPos2i(x, y);
		size_t l = (size_t)std::strlen(s);
		for (size_t i = 0; i < l; ++i)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i)); 
	}

	void Scene::DrawMenu(real* restitution, real* dynamicFriction, real* staticFriction)
	{
		if (!Menu) return; // Don't draw the menu if it's not active

		// Draw the menu background
		glColor4f(0.1f, 0.1f, 0.1f, 0.8f);
		glRectf(8, 3, 73, 50);

		// Draw the menu title and instructions
		glColor3f(1.0f, 1.0f, 1.0f);
		StrOnScene(33, 6, "Settings Menu", false);
		StrOnScene(12, 8, "Press 'M' to close menu", false);
		StrOnScene(12, 10, "'C' to go to deufault settings", false);
		StrOnScene(12, 12, "'U' to change surface/object settings", false);
		if (!ChangeSurface) 
		{
			if (custom)
			{
				// Draw custo object material settings
				StrOnScene(30, 16, "[Object - Custom]", false);
				StrOnScene(25, 18, "'T' to switch to Predefined", false);
				StrOnScene(23, 20, "Press 'R', 'D' or 'S' to change value", false);
				StrOnScene(12, 35, "R: Restitution", false);
				StrOnScene(12, 40, "D: Dynamic Friction", false);
				StrOnScene(12, 45, "S: Static Friction", false);

				// Display current custom material values
				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", *restitution);
				StrOnScene(40, 35, buffer, false);
				sprintf_s(buffer, "Dyn. Friction: %.2f", *dynamicFriction);
				StrOnScene(40, 40, buffer, false);
				sprintf_s(buffer, "Stat. Friction: %.2f", *staticFriction);
				StrOnScene(40, 45, buffer, false);
			}
			else
			{
				// Draw predefined object material settings
				StrOnScene(30, 16, "[Object - Predefined]", false);
				StrOnScene(23, 18, "Arrows right/left to change material", false);
				StrOnScene(25, 20, "'T' to switch to Custom", false);
				glColor3f(1.0f, 1.0f, 1.0f);
				StrOnScene(30, 22, "<- ", false);
				
				// Display selected predifined material and its properties
				for (int i = 0; i < materialCountObject; i++)
					if (i == selectedMaterialIndexObject)
					{
						glColor3f(0.0f, 1.0f, 0.0f);
						StrOnScene(33, 22, objectMaterials[i]->getName().c_str(), false);
						StrOnScene(33 + objectMaterials[i]->getName().size(), 22, " ->", false);
					}

				const Material* selectedMaterial = objectMaterials[selectedMaterialIndexObject]; 
				char buffer[50];
				sprintf_s(buffer, "Restitution: %.2f", selectedMaterial->getRestitution());
				*restitution = selectedMaterial->getRestitution();  
				StrOnScene(12, 35, buffer, false);
				sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
				*dynamicFriction = selectedMaterial->getDynamicFriction(); 
				StrOnScene(12, 40, buffer, false);
				sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
				*staticFriction = selectedMaterial->getStaticFriction();  
				StrOnScene(12, 45, buffer, false);
			}
		}
		else
		{
			// Draw predefined surface material settings
			StrOnScene(30, 16, "[Surface - Predefined]", false);
			StrOnScene(23, 18, "Arrows right/left to change material", false);
			glColor3f(1.0f, 1.0f, 1.0f); 
			StrOnScene(30, 20, "<- ", false);

			// Display selected predifined material and its properties
			for (int i = 0; i < materialCountSurface; i++) 
				if (i == selectedMaterialIndexSurface) 
				{
					glColor3f(0.0f, 1.0f, 0.0f); 
					StrOnScene(33, 20, surfaceMaterials[i]->getName().c_str(), false);
					StrOnScene(33 + surfaceMaterials[i]->getName().size(), 20, " ->", false);
				}

			const Material* selectedMaterial = surfaceMaterials[selectedMaterialIndexSurface];  
			char buffer[50];
			sprintf_s(buffer, "Restitution: %.2f", selectedMaterial->getRestitution());
			surfaceRB->SetRestitution(selectedMaterial->getRestitution()); 
			StrOnScene(12, 35, buffer, false);
			sprintf_s(buffer, "Dynamic Friction: %.2f", selectedMaterial->getDynamicFriction());
			surfaceRB->SetDynamicFriction(selectedMaterial->getDynamicFriction());
			StrOnScene(12, 40, buffer, false);
			sprintf_s(buffer, "Static Friction: %.2f", selectedMaterial->getStaticFriction());
			surfaceRB->SetStaticFriction(selectedMaterial->getStaticFriction());
			StrOnScene(12, 45, buffer, false);
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
		if (fread(header, 1, 54, f) != 54) 
		{
			std::cerr << "Not a correct BMP file: " << filename << std::endl;
			fclose(f);
			return nullptr;
		}
		// Extract image dimensions from the header
		width = *(int*)&(header[18]);
		height = *(int*)&(header[22]);
		int imageSize = width * height * 3; // 3 bytes per pixel for RGB

		// Allocate memory for the image data and read it
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
		std::cout << "Circle Material: " << rb->GetMaterial()->getName()
				  << ", Restitution: " << rb->GetRestitution()
			      << ", Dynamic Friction: " << rb->GetDynamicFriction()
			      << ", Static Friction: " << rb->GetStaticFriction() << std::endl << std::endl;

		ShapePolygon poly;
		poly.SetBox(30.0f, 1.0f);
		surfaceRB = Add(&poly, 40, 55);  
		material = surfaceMaterials[0]; 
		surfaceRB->SetMaterial(material);
		surfaceRB->SetStatic();
		surfaceRB->SetOrient(0);
		std::cout << "Polygon Material: " << surfaceRB->GetMaterial()->getName()
				  << ", Restitution: " << surfaceRB->GetRestitution()
			      << ", Dynamic Friction: " << surfaceRB->GetDynamicFriction()
			      << ", Static Friction: " << surfaceRB->GetStaticFriction() << std::endl << std::endl;

		ShapePolygon diagPoly;
		diagPoly.SetBox(10.0f, 1.0f);
		RigidBody* rbDiag = Add(&diagPoly, 70, 30);
		material = surfaceMaterials[1];
		rbDiag->SetMaterial(material);
		rbDiag->SetStatic(); 
		rbDiag->SetOrient(-PI / 6);
		std::cout << "Diagonal Polygon Material: " << rbDiag->GetMaterial()->getName()
				  << ", Restitution: " << rbDiag->GetRestitution()
			      << ", Dynamic Friction: " << rbDiag->GetDynamicFriction()
			      << ", Static Friction: " << rbDiag->GetStaticFriction() << std::endl << std::endl;
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
			contacts.erase(contacts.begin() + 0);
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
					if(!custom || !clickedCustomParameters)
						std::cout << "Polygon Material: " << rb->GetMaterial()->getName() 
								<< ", Restitution: " << rb->GetRestitution()
								<< ", Dynamic Friction: " << rb->GetDynamicFriction()
								<< ", Static Friction: " << rb->GetStaticFriction() << std::endl << std::endl; 
					else if(custom && clickedCustomParameters)
						std::cout << "Polygon Custom Material: "
								  << " Restitution: " << rb->GetRestitution()
								  << ", Dynamic Friction: " << rb->GetDynamicFriction()
								  << ", Static Friction: " << rb->GetStaticFriction() << std::endl << std::endl;
					delete[] vectors;
				}
				break;
				case GLUT_RIGHT_BUTTON:
				{
					// Create and add a random circle
					ShapeCircle circle(Random(1.0f, 5.0f));
					RigidBody* rb = Add(&circle, (size_t)x_, (size_t)y_, selectedMaterialIndexObject);
					if (!custom || !clickedCustomParameters) 
						std::cout << "Circle Material: " << rb->GetMaterial()->getName() 
								  << ", Restitution: " << rb->GetRestitution() 
								  << ", Dynamic Friction: " << rb->GetDynamicFriction() 
								  << ", Static Friction: " << rb->GetStaticFriction() << std::endl << std::endl; 
					else if(custom && clickedCustomParameters)
						std::cout << "Circle Custom Material: "
								  << " Restitution: " << rb->GetRestitution() 
								  << ", Dynamic Friction: " << rb->GetDynamicFriction()  
								  << ", Static Friction: " << rb->GetStaticFriction() << std::endl << std::endl; 
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
		StrOnScene(1, 2, "'Esc' to exit", true);
		StrOnScene(1, 4, "Left click add a polygon, Right click add a circle",true);
		StrOnScene(1, 6, "Press 'M' to open menu",true);
		StrOnScene(1, 8, "'S' to start/stop the simulation, 'F' to frame by frame", true);
		StrOnScene(1, 10, "'C' to clear the scene",true);
		//char buffer[250];
		//sprintf_s(buffer, "Press 'G' to increase gravity, 'H' to decrease gravity", GRAVITY_S);  
		//StrOnScene(1, 14, buffer, true); 

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
			DrawMenu(&restitution, &dynamicFriction, &staticFriction);
		else
			DrawMenu(nullptr, nullptr, nullptr); 

		glutSwapBuffers(); // Swap the buffers
	}

	// Main loop for the program
	void Scene::mainLoop()
	{
		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (showLogo)
			DisplayLogo(); // Display the logo  
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
						{
							selectedMaterialIndexObject++;
							if (selectedMaterialIndexObject == GetmaterialCountObject())
								selectedMaterialIndexObject = 0;
						}
					}
					else
					{
						custom = false;
						selectedMaterialIndexSurface++;
						if (selectedMaterialIndexSurface == GetmaterialCountSurface())
							selectedMaterialIndexSurface = 0;
					}
				}
				break;
				case GLUT_KEY_LEFT:
				{
					if (!custom && !ChangeSurface)
					{
						selectedMaterialIndexObject--;
						if (selectedMaterialIndexObject < 0)
							selectedMaterialIndexObject = GetmaterialCountObject() - 1;
					}
					else if (ChangeSurface)
					{
						custom = false;
						selectedMaterialIndexSurface--;
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
					clickedCustomParameters = true;
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
					clickedCustomParameters = true;
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
					clickedCustomParameters = true;
					staticFriction += 0.05f;
					if (staticFriction > 1.01f)
						staticFriction = 0.05f;
				}
			}
			break;
			case 'c':
			case 'C': // reset values to default
			{
				clickedCustomParameters = false;
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
				clickedCustomParameters = false;
				if (!ChangeSurface)
					custom = !custom;
			}
			break;
			case 'm':
			case 'M': // Close the menu
			{
				Menu = false;
				if (!Menu)
					frameStep = false;
				std::cout << "Surface Material: " << surfaceRB->GetMaterial()->getName()
						  << ", Restitution: " << surfaceRB->GetRestitution()
					      << ", Dynamic Friction: " << surfaceRB->GetDynamicFriction()
					      << ", Static Friction: " << surfaceRB->GetStaticFriction() << std::endl << std::endl;

			}
			default:
			{
				if ((key != 'm' && key != 'M')) 
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
				{
					exit(0);
				}
				break;
				case 'm':
				case 'M': // Open/Close the menu
				{
					if (!showLogo)
					{
						Menu = true;
						frameStep = true;
					}
				}
				break;
				case 'c':
				case 'C': // Clear the scene
				{
					if (!showLogo)
						Clear();
				}
				break;
				case 's':
				case 'S': // Start/Stop the simulation
				{
					if (!showLogo)
						frameStep = frameStep ? false : true;
				}
				break;
				case 'f':
				case 'F': // Step through the simulation frame by frame
				{	
					if (!showLogo)
						step = true;
				}
				// will be added later
				
				//case 'g':
				//case 'G': // gravity up
				//{

				//}
				//case 'h':
				//case 'H': // gravity down
				//{

				//}
				//case 'j':
				//case 'J': // change gravity direction
				//{
				//	
				//}
				//break;
				default:
				{
					if(key != 'f' && key != 'F')
						std::cout << "Wrong key pressed..." << std::endl;
				}
				break;
			}
		}
	}
}