#include <GL/glut.h>
#include <iostream>
#include "input.h"
#include "ship.h"

extern Ship *playerShip;

void readKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 'Q':
			exit(0);
			break;

		case 'e':
		case 'E':
			playerShip->accelerate();
			break;

		case 'd':
		case 'D':
			playerShip->decelerate();
			break;

		case 's':
		case 'S':
			playerShip->yawLeft();
			break;

		case 'f':
		case 'F':
			playerShip->yawRight();
			break;

		case 'g':
		case 'G':
			playerShip->stabilize();
			break;
	}

	glutPostRedisplay();
}

void readSpecialKeys(int key, int x, int y)
{
//	glutPostRedisplay();
}


void mouseButtHandler(int button, int state, int x, int y)
{
//	glutPostRedisplay();
}


void mouseMoveHandler(int x, int y)
{
//	glutPostRedisplay();
}

