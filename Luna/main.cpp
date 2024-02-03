#include "src/Luna/Libs.h"



void mouse(int button, int state, int x, int y)
{
	
}

void keyboard(unsigned char key, int x, int y)
{
	
}


void LunaLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the depth buffer
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

	srand(1); // Seed the random number generator
	glutMainLoop(); // Start the main loop
}