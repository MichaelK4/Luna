#include "Libs.h"


Luna::Scene scene(1.0f / 60.0f, 10);
Luna::Clock clock_time;
bool frameStep = false;
bool step = false;

void mouse(int button, int state, int x, int y)
{
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
			Luna::RigidBody* rb = scene.Add(&poly, x, y);
			rb->SetOrient(Luna::Random(-PI, PI));
			rb->Restitution = 0.2f;
			rb->DynamicFriction = 0.2f;
			rb->StaticFriction = 0.4f;
			delete[] vectors;
		}
		break;
		case GLUT_RIGHT_BUTTON:
		{
			Luna::Circle circle(Luna::Random(1.0f, 3.0f));
			int rd = (int)Luna::Random(0, 2); // Random number between 0 and 1
			if (rd == 0)
			{
				Luna::RigidBody* rb = scene.Add(&circle, x, y);
				rb->Restitution = 0.2f;
				rb->DynamicFriction = 0.2f;
				rb->StaticFriction = 0.4f;
			}
			else
			{
				printf("In Soft Body\n");
				Luna::SoftBody* sb = scene.AddSoft(&circle, x, y);
				sb->Restitution = 5.0f;
				sb->DynamicFriction = 0.2f;
				sb->StaticFriction = 0.4f;
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

void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case ESC:
		exit(0);
		break;

	case GLUT_KEY_LEFT:
		frameStep = frameStep ? false : true;
		break;

	case GLUT_KEY_RIGHT:
		step = true;
		break;
	}
}

void StrOnScene(int x, int y, const char* s)
{
	glColor3f(0.5f, 0.5f, 0.9f);
	glRasterPos2i(x, y);
	uint l = (uint)std::strlen(s);
	for (uint i = 0; i < l; ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
}

void LunaLoop()
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set the clear color to a light grey
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the depth buffer

	StrOnScene(1, 2, "Press 'Esc' to exit");
	StrOnScene(1, 4, "Left click add a polygon");
	StrOnScene(1, 6, "Right click add a circle");
	//StrOnScene(1, 6, "Press 'space' to add a triangle");

	static real accumulator = 0.0f;

	accumulator += clock_time.Elapsed(); // Get the elapsed time since the last frame

	clock_time.Start(); // Start the clock

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
	glutSwapBuffers();
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (only single buffered for now)

	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	//glutInitWindowSize(screenWidth, screenHeight); // Set the width and height of the window
	//glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600); // Set the width and height of the window
	glutInitWindowPosition(350, 100); // Set the position of the window
	glutCreateWindow("Luna2D - Physics Engine"); // Set the title for the window

	srand(time(NULL)); // Seed the random number generator

	// Full Screen
	//glutFullScreen();

	glutDisplayFunc(LunaLoop); // Tell GLUT to use the method "LunaLoop" for rendering
	glutKeyboardFunc(keyboard); // Tell GLUT to use the method "keyboard" for key presses
	glutMouseFunc(mouse); // Tell GLUT to use the method "mouse" for mouse events
	glutIdleFunc(LunaLoop); // Tell GLUT to use the method "LunaLoop" for updates

	glMatrixMode(GL_PROJECTION); // Set the matrix mode to GL_PROJECTION
	glPushMatrix(); // Push the matrix
	glLoadIdentity(); // Load the identity
	//gluOrtho2D(0, screenWidth, screenHeight, 0);
	gluOrtho2D(0, 80, 60, 0); // Set the orthogonal view to a 80x60 area
	glMatrixMode(GL_MODELVIEW); // Set the matrix mode to GL_MODELVIEW
	glPushMatrix(); // Push the matrix 
	glLoadIdentity(); // Load the identity



	Luna::Circle circle(5.0f);
	Luna::RigidBody* rb = scene.Add(&circle, 40, 40);
	rb->SetStatic();

	Luna::ShapePolygon poly;
	poly.SetBox(30.0f, 1.0f);
	rb = scene.Add(&poly, 40, 55);
	rb->SetStatic();
	rb->SetOrient(0);

	Luna::ShapePolygon diagPoly;
	Luna::Vector2 diagVec[4] = { Luna::Vector2(-1, -1), Luna::Vector2(1, -1),
								Luna::Vector2(8, 8), Luna::Vector2(-8, 8) };
	diagPoly.Set(diagVec, 4);
	Luna::RigidBody* rbDiag = scene.Add(&diagPoly, 70, 30);
	rbDiag->SetOrient(PI / 4);
	rbDiag->SetStatic();


	//srand(1); // Seed the random number generator
	glutMainLoop(); // Start the main loop
}