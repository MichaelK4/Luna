#include <iostream>
#include <time.h>
#include <math.h>
#include "glut.h"


void init()
{
	glClearColor(0,0.5,0.8,0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	glutSwapBuffers(); // show all
}

void idle() 
{
	glutPostRedisplay();
}


void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}