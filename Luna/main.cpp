#include "Libs.h"
Luna::Scene scene(1.0f / 60.0f, 10); // Create a scene with a time step of 1/60 and 10 iterations 

// Call the mainLoop function in the scene
void mainLoop() { scene.mainLoop(); }

// Call the keyboardArrowKeys function in the scene
void keyboardArrowKeys(int key, int x, int y) { scene.keyboardArrowKeys(key, x, y); }

// Call the LunaLoop function in the scene
void LunaLoop() { scene.LunaLoop(); }

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

// Call the mouse function in the scene
void mouse(int button, int state, int x, int y) { scene.mouse(button, state, x, y); } 

// Main function
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (double buffered)

	// Set window position and size
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Luna2D - Physics Engine"); // Set the window title

	srand(time(NULL)); // Seed the random number generator 

	scene.LoadLogoTexture("Luna2D-Logo.bmp"); // Load the logo texture

	// Set up GLUT callback functions
	glutDisplayFunc(mainLoop); // Set the display callback function
	glutKeyboardFunc(keyboard); // Set the keyboard callback function
	glutSpecialFunc(keyboardArrowKeys); // Set the keyboard arrow keys callback function
	glutMouseFunc(mouse); // Set the mouse callback function
	glutIdleFunc(mainLoop); // Set the idle callback function

	// Set up OpenGL projection
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0, 80, 60, 0); 
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();

	scene.Init(); // Set up OpenGL
	scene.ShowOnDisplay(); // Show static objects on display

	glutMainLoop(); // Enter the GLUT main loop

	scene.Clear(); // Clean up the program 
	return 0; 
}