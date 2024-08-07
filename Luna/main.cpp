#include "Libs.h"
Luna::Scene scene(DELTA_TIME, ITERATIONS); // Create a scene with a time step of 1/60 and 10 iterations  

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
	else scene.keyboard(key, x, y); 
}

void reshape(int w, int h) { glutReshapeWindow(WIDTH, HEIGHT); } 

// Call the mouse function in the scene
void mouse(int button, int state, int x, int y) { scene.mouse(button, state, x, y); } 

// Main function
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (double buffered)

	// Set window position and size
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(POX_X, POX_Y);
	glutCreateWindow(TITLE); // Set the window title

	srand(time(NULL)); // Seed the random number generator 

	scene.LoadLogoTexture(LOGO); // Load the logo texture

	// Set up GLUT callback functions
	glutDisplayFunc(mainLoop); // Set the display callback function
	glutKeyboardFunc(keyboard); // Set the keyboard callback function
	glutSpecialFunc(keyboardArrowKeys); // Set the keyboard arrow keys callback function
	glutMouseFunc(mouse); // Set the mouse callback function
	glutIdleFunc(mainLoop); // Set the idle callback function

	glutReshapeFunc(reshape);

	// Set up OpenGL projection
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(LEFT_ORTHO, RIGHT_ORTHO, BOTTOM_ORTHO, TOP_ORTHO); // Set up the orthographic projection
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();

	scene.Init(); // Set up OpenGL
	scene.ShowOnDisplay(); // Show static objects on display

	glutMainLoop(); // Enter the GLUT main loop

	scene.Clear(); // Clean up the program 
	return 0; 
}