#include "Shape.h"
#include "Scene.h"
#include "Clock.h"


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
			size_t count = (size_t)Luna::Random(4, 4);
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
			Luna::RigidBody* rb = scene.Add(&circle, x, y);
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
	glColor3d(0.5f, 0.5f, 0.9f);
	glRasterPos2i(x, y);
	uint l = (uint)std::strlen(s);
	for (uint i = 0; i < l; ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
}

void LunaLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the depth buffer

	StrOnScene(1, 2, "Left click add a polygon");
	StrOnScene(1, 4, "Right click add a circle");
	//StrOnScene(1, 6, "Press 'space' to add a triangle");

	static double accumulator = 0;

	accumulator += clock_time.Elapsed(); // Get the elapsed time since the last frame

	clock_time.Start(); // Start the clock

	accumulator = Luna::Clamp(0.0f, 0.1f, accumulator); // Clamp the accumulator

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
	glutInitWindowSize(800, 600); // Set the width and height of the window
	glutInitWindowPosition(350, 100); // Set the position of the window
	glutCreateWindow("Luna2D"); // Set the title for the window
	
	glutDisplayFunc(LunaLoop); // Tell GLUT to use the method "LunaLoop" for rendering
	glutKeyboardFunc(keyboard); // Tell GLUT to use the method "keyboard" for key presses
	glutMouseFunc(mouse); // Tell GLUT to use the method "mouse" for mouse events
	glutIdleFunc(LunaLoop); // Tell GLUT to use the method "LunaLoop" for updates

	glMatrixMode(GL_PROJECTION); // Set the matrix mode to GL_PROJECTION
	glPushMatrix(); // Push the matrix
	glLoadIdentity(); // Load the identity
	gluOrtho2D(0, 80, 60, 0); // Set the orthogonal view to a 80x60 area
	glMatrixMode(GL_MODELVIEW); // Set the matrix mode to GL_MODELVIEW
	glPushMatrix(); // Push the matrix 
	glLoadIdentity(); // Load the identity

	Luna::Circle circle(5.0f);
	Luna::RigidBody* rb = scene.Add(&circle, 40, 30); 
	rb->SetStatic();  

	Luna::ShapePolygon poly; 
	poly.SetBox(30.0f, 1.0f);
	rb = scene.Add(&poly, 40, 50); 
	rb->SetStatic(); 
	rb->SetOrient(0); 

	srand(1); // Seed the random number generator
	glutMainLoop(); // Start the main loop
}