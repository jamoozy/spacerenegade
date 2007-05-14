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
#include "input.h"
#include "camera.h"
#include "asteroid.h"
//#include "globals.h"
#include "ship.h"
#include "environment.h"

#define DEBUG_MODE 1
#define PRINT_FPS 0
static int FPS;
static int MSPF;

#if (DEBUG_MODE)
	int window;
#endif

Ship *playerShip;
OctTree *env;


#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768

#if (PRINT_FPS)
time_t last_time;
int frames_this_second;
#endif

struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;

void cleanup()
{
	#if (!DEBUG_MODE)
		glutLeaveGameMode();
	#endif
	Camera::cleanUp();
	Keyboard::cleanUp();
	Mouse::cleanUp();
	delete env;
	exit(0);
}

void handleInput()
{
	if (Keyboard::getKeyboard()->isDown(SR_KEY_Q))
		cleanup();

	// Yaw.
	if (Keyboard::getKeyboard()->isDown(SR_KEY_S))
		playerShip->yawLeft();
	if (Keyboard::getKeyboard()->isDown(SR_KEY_F))
		playerShip->yawRight();

	// Acceleration.
	if (Keyboard::getKeyboard()->isDown(SR_KEY_SPACE))
		playerShip->accelerate();
	if (Keyboard::getKeyboard()->isDown(SR_KEY_A))
		playerShip->decelerate();
	if (Keyboard::getKeyboard()->isDown(SR_KEY_G))
		playerShip->stabilize();

	// Pitch.
	if (Keyboard::getKeyboard()->isDown(SR_KEY_D))
		playerShip->pitchBack();
	if (Keyboard::getKeyboard()->isDown(SR_KEY_E))
		playerShip->pitchForward();
}


//##########################################
// OpenGL Display function

void doNextFrame(int value)
{
	glutPostRedisplay();
	glutTimerFunc(MSPF, doNextFrame, 0);
}

void display(void)
{
	#if (DEBUG_MODE)
		glutSetWindow(window);
	#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	adjustCamera();

	handleInput();

	#if (PRINT_FPS)
		if (last_time != time(NULL))
		{
			std::cout << ctime(&last_time) << "fps: " << frames_this_second << std::endl;
//			int difference = frames_this_second - FPS;
			last_time = time(NULL);
			frames_this_second = 0;
		}
		else
			frames_this_second++;
	#endif

	env->update();

	glutSwapBuffers();
}


//##########################################
// Init display settings

void initDisplay()
{
	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = FPS;
	#endif

	// Perspective projection parameters
	pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 500.0;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glTranslated(0,0,0);
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearIndex(0);
	glClearDepth(1);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//GLfloat direction[] = {0.0f,-1.0f,0.0f,0.0f};
	//GLfloat color[] = {0.0f,0.0f,1.0f};

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
	//glLightfv(GL_LIGHT0, GL_POSITION, direction);
	//glLightfv(GL_LIGHT0, GL_AMBIENT,  color);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE,  color);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, color);
}

//##########################################
// Main function

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	FPS = 60;
	MSPF = 17;

	env = new OctTree();
	env->initLeaves();

	Asteroid *asteroids[6];
	asteroids[0] = new Asteroid( 0, 0,-59 ,  0.00,0.00,-0.00);
	env->add(asteroids[0]);
	asteroids[1] = new Asteroid( 0, 0,-41 ,  0.00,0.00,-0.00);
	env->add(asteroids[1]);
	asteroids[2] = new Asteroid( 0,-9,-50 ,  0.00,0.00,-0.00);
	env->add(asteroids[2]);
	asteroids[3] = new Asteroid( 0, 9,-50 ,  0.00,0.00,-0.00);
	env->add(asteroids[3]);
	asteroids[4] = new Asteroid(-9, 0,-50 ,  0.00,0.00,-0.00);
	env->add(asteroids[4]);
	asteroids[5] = new Asteroid( 9, 0,-50 ,  0.00,0.00,-0.00);
	env->add(asteroids[5]);

	playerShip = new Ship();
	playerShip->setAt(0,0,-30);
	env->add(playerShip);

	srand(time(NULL));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	#if (DEBUG_MODE)
		glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT); 
		window = glutCreateWindow("Space Renegade"); 
	#else
		glutEnterGameMode();
		#ifndef WIN32
			glutGameModeString("1024x768:32@60");
		#else
			glutGameModeString("width=1024;height=768;bpp=32;");
		#endif
	#endif

	glutTimerFunc(MSPF, doNextFrame, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(readKeyboard);
	glutKeyboardUpFunc(readKeyboardUp);
	glutSpecialFunc(readSpecialKeys);
	glutSpecialUpFunc(readSpecialKeysUp);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutIdleFunc(NULL);

	initDisplay();

	glutMainLoop();

	return 0;
}
