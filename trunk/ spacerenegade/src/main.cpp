#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glut.h>
#include <AL/alut.h>
#include "input.h"
#include "camera.h"
#include "asteroid.h"
#include "planet.h"
#include "globals.h"
#include "ship.h"
#include "environment.h"
#include "button.h"
#include "display.h"
#include "menu.h"

using std::cout;
using std::endl;


#if (DEBUG_MODE)
	int window;
#endif

PShip *playerShip;  // Jam: The player's ship, duh.
OctTree *env;		// Jam: Collision detection of objects and the world
					//      (environment) in general.
OctTree *zoom2x;
OctTree *zoomx;
Menu *menu;        // Gum: The current menu of buttons

#if (PRINT_FPS || LIMIT_FPS)
	int FPS;
	int MSPF;
#endif

// Jam:
// Put any deletes in here.  Make sure that the value is only deleted
// if it's actually pointing to something (i.e. "if (x) delete x").
void cleanup()
{
	#if (!DEBUG_MODE)
		glutLeaveGameMode();
	#endif
	Camera::cleanUp();
	Keyboard::cleanUp();
	Mouse::cleanUp();
	if (env) delete env;
	if (menu) delete menu;
	alutExit();
	exit(0);
}

#if (LIMIT_FPS)
	// This is used to control the framerate.
	void doNextFrame(int value)
	{
		glutPostRedisplay();
		glutTimerFunc(MSPF, doNextFrame, 0);
	}
#endif

// Main function
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	alutInit(&argc, argv);

	#if (PRINT_FPS || LIMIT_FPS)
		FPS = 60;
		MSPF = 17;
	#endif

	env = NULL;
	playerShip = NULL;
	menu = NULL;

	srand(time(NULL));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	#if (DEBUG_MODE)
		glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT); 
		glutInitWindowPosition(0, 0);
		window = glutCreateWindow("Space Renegade"); 
	#else
		glutEnterGameMode();
		#ifndef WIN32
			glutGameModeString("1024x768:32@60");
		#else
			glutGameModeString("width=1024;height=768;bpp=32;");
		#endif
	#endif

	#if (LIMIT_FPS)
		glutTimerFunc(MSPF, doNextFrame, 0);
		glutIdleFunc(NULL);
	#else
		glutIdleFunc(display);
	#endif	
	
	initStartScreen();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(readKeyboard);
	glutKeyboardUpFunc(readKeyboardUp);
	glutSpecialFunc(readSpecialKeys);
	glutSpecialUpFunc(readSpecialKeysUp);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	
	glutMainLoop();
	
	return 0;
}
