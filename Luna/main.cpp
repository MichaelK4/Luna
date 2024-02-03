#include "Libs.h"



void mouse(int button, int state, int x, int y)
{
	
}

void keyboard(unsigned char key, int x, int y)
{
	
}


void LunaLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Luna2D");
	
	glutDisplayFunc(LunaLoop);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutDisplayFunc(display);
	glutIdleFunc(LunaLoop);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 80, 60, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//init();
	srand(1);
	glutMainLoop();
}