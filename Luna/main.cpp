#include "Libs.h"
Luna::Scene scene(1.0f / 60.0f, 10); // Create a scene with a time step of 1/60 and 10 iterations 

void mainLoop()
{
	scene.mainLoop();
}

void keyboardArrowKeys(int key, int x, int y)
{
	scene.keyboardArrowKeys(key, x, y); 
}

void LunaLoop() 
{
	scene.LunaLoop();
}

void keyboard(unsigned char key, int x, int y)
{
	if (scene.getIsShowLogo() && key == ' ')
	{
		scene.setShowLogo(false);  
		glutDisplayFunc(LunaLoop);
		glutPostRedisplay();
	}
	else
		scene.keyboard(key, x, y); 
}

void mouse(int button, int state, int x, int y) 
{
	scene.mouse(button, state, x, y);  
} 

// Main function
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (double buffered)

	// Set window position and size
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT); 
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Luna2D - Physics Engine"); // Set the window title

	srand(time(NULL)); // Seed the random number generator 

	scene.LoadLogoTexture("Luna2D-Logo.bmp"); // Load the logo texture

	// Set up GLUT callback functions
	glutDisplayFunc(mainLoop);  
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(keyboardArrowKeys);
	glutMouseFunc(mouse);
	glutIdleFunc(mainLoop); 

	// Set up OpenGL projection
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0, 80, 60, 0); 
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();

	scene.Init(); // Set up OpenGL
	scene.ShowOnDisplay();

	glutMainLoop(); // Enter the GLUT main loop

	scene.Clear(); // Clean up the program 
	return 0; 
}