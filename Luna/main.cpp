#include "Libs.h"

real restitution;// = 0.2f; // Restitution is the bounciness of an object
real dynamicFriction;// = 0.3f; // Dynamic friction is less than static friction
real staticFriction;// = 0.5f; // Static friction is greater than dynamic friction

bool showLogo = true; // Show the logo at the start of the program

Luna::Scene scene(1.0f / 60.0f, 10); // Create a scene with a time step of 1/60 and 10 iterations
Luna::Clock clock_time; // Create a clock to keep track of the time
bool frameStep = false; // Step through the simulation frame by frame
bool step = false; // Step through the simulation
bool Menu = false; // Show the menu
bool custom = false; // Custom values for restitution, dynamic friction, and static friction
bool ChangeSurface = false; // Change the surface material instead of the object material
int selectedMaterialIndexObject = 0; // Index for the selected material for the object
int selectedMaterialIndexSurface = 0; // Index for the selected material for the surface


void mouse(int button, int state, int x, int y)
{
	if(Menu) return; // If the menu is open, don't add objects
	if(showLogo) return; // If the logo is showing, don't add objects
	x /= 10.0f;
	y /= 10.0f;

	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
		{
			Luna::ShapePolygon poly;
			size_t count = (size_t)Luna::Random(3, MaxPolyCount);
			Luna::Vector2* vectors = new Luna::Vector2[count];
			real radius = Luna::Random(5, 10);
			for (size_t i = 0; i < count; i++)
				vectors[i].Set(Luna::Random(-radius, radius), Luna::Random(-radius, radius));
			poly.Set(vectors, count);
			//Luna::RigidBody* rb = scene.Add(&poly, x, y);
			Luna::RigidBody* rb = scene.Add(&poly, x, y, selectedMaterialIndexObject);
			rb->SetOrient(Luna::Random(-PI, PI));
			//rb->SetRestitution(restitution);
			//rb->SetDynamicFriction(dynamicFriction);
			//rb->SetStaticFriction(staticFriction);
			std::cout << "Poly: Restitution: " << rb->GetRestitution() << " Dynamic Friction: " << rb->GetDynamicFriction() << " Static Friction: " << rb->GetStaticFriction() << std::endl;
			delete[] vectors;
		}
		break;
		case GLUT_RIGHT_BUTTON:
		{
			Luna::Circle circle(Luna::Random(1.0f, 3.0f));
			int rd = (int)Luna::Random(0, 2); // Random number between 0 and 1
			rd = 0; // for now
			if (rd == 0)
			{
				//Luna::RigidBody* rb = scene.Add(&circle, x, y);
				Luna::RigidBody* rb = scene.Add(&circle, x, y, selectedMaterialIndexObject); 
				//rb->SetStaticFriction(staticFriction);
				//rb->SetDynamicFriction(dynamicFriction);
				//rb->SetRestitution(restitution); 
				std::cout << "Circle: Restitution: " << rb->GetRestitution() << " Dynamic Friction: " << rb->GetDynamicFriction() << " Static Friction: " << rb->GetStaticFriction() << std::endl;
				//rb->Restitution = restitution;
				//rb->DynamicFriction = dynamicFriction;
				//rb->StaticFriction = staticFriction;
			}
			else
			{
				//printf("In Soft Body\n");
				//Luna::SoftBody* sb = scene.AddSoft(&circle, x, y);
				//sb->Restitution = 0.2f;
				//sb->DynamicFriction = 0.2f;
				//sb->StaticFriction = 0.4f;
				//sb->velocity = Luna::Vector2(Luna::Random(-10, 10), Luna::Random(-10, 10));
				//sb->AngularVelocity = Luna::Random(-10, 10); 

				//sb->Damping = 3.0f;//rand() % 100 / 1000.0f + 0.1; // Random number between 0.1 and 0.2
				// rand between 0.1 and 1.0
				//sb->Softness = rand() % 1000 / 1000.0f + 0.1; // Random number between 0.1 and 1.0
				//sb->Update(deltaTime);
			}
		}
		break;
		}
	}
}

void LunaLoop()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set the clear color to a light grey
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the depth buffer

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 80, 60, 0); // Set orthogonal projection for simulation

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	scene.StrOnScene(1, 2, "Press 'Esc' to exit");
	scene.StrOnScene(1, 4, "Left click add a polygon");
	scene.StrOnScene(1, 6, "Right click add a circle");
	scene.StrOnScene(1,8, "Press 'M' to open menu");
	scene.StrOnScene(1, 10, "Press 'D' to start/stop the simulation");
	scene.StrOnScene(1, 12, "Press 'C' to clear the scene");

	static real accumulator = 0.0f;

	accumulator += clock_time.Elapsed(); // Get the elapsed time since the last frame

	clock_time.Start(); // Start the clock

	//acmulator is the time that has passed since the last frame
	accumulator = Luna::Clamp(accumulator, 0.0f, 0.1f); // Clamp the accumulator

	while (accumulator >= deltaTime) // If the accumulator is greater than the time step, step the scene
	{
		if (!frameStep)
		{
			scene.Step();
		}
		else
		{
			if (step)
			{
				scene.Step();
				step = false;
			}
		}
		accumulator -= deltaTime;
	}

	clock_time.Stop(); // Stop the clock

	scene.Render();
	//real* r = &restitution;
	//real* d = &dynamicFriction;
	//real* s = &staticFriction;
	if(!ChangeSurface)
		scene.DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexObject, &restitution, &dynamicFriction, &staticFriction);
	else
		scene.DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexSurface, &restitution, &dynamicFriction, &staticFriction);
	//restitution = *r;
	//dynamicFriction = *d;
	//staticFriction = *s;
	// DEBUG REASONS
	//std::cout<< "Restitution: " << restitution << " Dynamic Friction: " << dynamicFriction << " Static Friction: " << staticFriction << std::endl;
	glutSwapBuffers();
}

void mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if(showLogo)
		scene.DisplayLogo(); // Display the logo
	else
		LunaLoop(); // Display the simulation
}

void KeyboardMenu(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		if (custom && !ChangeSurface)
		{
			restitution += 0.05f;
			if (restitution > 1.01f)
				restitution = 0.05f;
		}
		break;
	case 'd':
		if (custom && !ChangeSurface)
		{
			dynamicFriction += 0.05f;
			if (dynamicFriction > 1.01f)
				dynamicFriction = 0.05f;
		}
		break;
	case 'u':
		ChangeSurface = !ChangeSurface;
		break;
	case 's':
		if (custom && !ChangeSurface)
		{
			staticFriction += 0.05f;
			if (staticFriction > 1.01f) 
				staticFriction = 0.05f;
		}
		break;

	case 'c': // reset values to default
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
		break;
	case 't': // toggle custom values
		if(!ChangeSurface)
			custom = !custom;
		break;
	case 'n': // next
		if (!ChangeSurface)
		{
			if (!custom && !ChangeSurface)
				selectedMaterialIndexObject = (selectedMaterialIndexObject + 1) % scene.GetmaterialCountObject();
		}
		else if (ChangeSurface)
		{
			custom = false;
			selectedMaterialIndexSurface = (selectedMaterialIndexSurface + 1) % scene.GetmaterialCountSurface();
		}
		break;

	case 'p': // previous
		if (!custom && !ChangeSurface)
		{
			selectedMaterialIndexObject = (selectedMaterialIndexObject - 1) % scene.GetmaterialCountObject();
			if(selectedMaterialIndexObject < 0)
				selectedMaterialIndexObject = scene.GetmaterialCountObject() - 1;
		}
		else if(ChangeSurface)
		{
			custom = false;
			selectedMaterialIndexSurface = (selectedMaterialIndexSurface - 1) % scene.GetmaterialCountSurface();
			if (selectedMaterialIndexSurface < 0) 
				selectedMaterialIndexSurface = scene.GetmaterialCountSurface() - 1; 
		}
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (Menu)
	{
		KeyboardMenu(key, x, y);
	}

	switch (key)
	{
	case ESC:
		exit(0);
		break;
	case 'm':
		if (!showLogo)
		{
			Menu = !Menu;
			if (Menu) { frameStep = true; }
			else { frameStep = false; }
		}
		break;
	case 'c':
		if (!Menu && !showLogo)
			scene.Clear();
		break;
	case ' ':
		if (showLogo)
		{
			//std::cout << "Logo is showing" << std::endl; 
			showLogo = false;
			glutDisplayFunc(LunaLoop);
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_LEFT:
		if (!Menu && !showLogo)
			frameStep = frameStep ? false : true;
		break;

	case GLUT_KEY_RIGHT:
		if (!Menu && !showLogo)
			step = true;
		break;
	}
}

void Clean()
{
	scene.Clear();
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (only single buffered for now)

	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT); 
	glutInitWindowSize(WIDTH, HEIGHT); // Set the width and height of the window 
	glutInitWindowPosition(350, 100); // Set the position of the window
	glutCreateWindow("Luna2D - Physics Engine"); // Set the title for the window

	srand(time(NULL)); // Seed the random number generator

	scene.LoadLogoTexture("Luna2D-Logo.bmp"); // Load the logo texture

	glutDisplayFunc(mainLoop); // Tell GLUT to use the method "display" for rendering
	glutKeyboardFunc(keyboard); // Tell GLUT to use the method "keyboard" for key presses
	glutMouseFunc(mouse); // Tell GLUT to use the method "mouse" for mouse events
	glutIdleFunc(mainLoop); // Tell GLUT to use the method "mainLoop" for the idle function

	glMatrixMode(GL_PROJECTION); // Set the matrix mode to GL_PROJECTION
	glPushMatrix(); // Push the matrix
	glLoadIdentity(); // Load the identity
	gluOrtho2D(0, 80, 60, 0); // Set the orthogonal view to a 80x60 area
	glMatrixMode(GL_MODELVIEW); // Set the matrix mode to GL_MODELVIEW
	glPushMatrix(); // Push the matrix 
	glLoadIdentity(); // Load the identity

	scene.Init(); // Set up OpenGL

	scene.ShowOnDisplay();
	
	glutMainLoop(); // Start the main loop

	Clean(); // Clean up the program
}