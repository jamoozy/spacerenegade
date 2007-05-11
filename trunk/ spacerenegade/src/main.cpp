#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
//#include <signal.h>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glut.h>
#include "input.h"
#include "camera.h"
#include "asteroid.h"
#include "globals.h"

#define DEBUG_MODE 1
#define PRINT_FPS 0
#define FPS 60

#if (DEBUG_MODE)
	int window;
#endif


#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768

time_t last_time;
int frames_this_second;
int frame_staller;

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
	delete [] asteroids;
	delete playerShip;
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

void display(void)
{
	for (int i = 0; i < frame_staller; i++);

	#if (DEBUG_MODE)
		glutSetWindow(window);
	#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	adjustCamera();

	handleInput();

	if (last_time != time(NULL))
	{
		#if (PRINT_FPS)
			std::cout << ctime(&last_time) << "fps: " << frames_this_second;
			std::cout << "    staller: " << frame_staller << std::endl;
		#endif
		int difference = frames_this_second - FPS;
		frame_staller += 4000 * (difference);
		last_time = time(NULL);
		frames_this_second = 0;
	}
	else
		frames_this_second++;


	for (int i = 0; i < 6; i++)
		asteroids[i].draw();

	playerShip->draw();

	glutSwapBuffers();
}


//##########################################
// Init display settings

void initDisplay()
{
	last_time = 0;
	frames_this_second = FPS;

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

	asteroids = new Asteroid[6];
	asteroids[0] = Asteroid( 0, 0,-59 ,  0.00,0.00,-0.00);
	asteroids[1] = Asteroid( 0, 0,-41 ,  0.00,0.00,-0.00);
	asteroids[2] = Asteroid( 0,-9,-50 ,  0.00,0.00,-0.00);
	asteroids[3] = Asteroid( 0, 9,-50 ,  0.00,0.00,-0.00);
	asteroids[4] = Asteroid(-9, 0,-50 ,  0.00,0.00,-0.00);
	asteroids[5] = Asteroid( 9, 0,-50 ,  0.00,0.00,-0.00);

	playerShip = new Ship();
	playerShip->setAt(0,0,-30);

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
	//signal(SIGHUP, cleanup);

	frame_staller = 8000000;

	glutInit(&argc, argv);

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

	glutDisplayFunc(display);
	glutKeyboardFunc(readKeyboard);
	glutKeyboardUpFunc(readKeyboardUp);
	glutSpecialFunc(readSpecialKeys);
	glutSpecialUpFunc(readSpecialKeysUp);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutIdleFunc(display);

	initDisplay();

	glutMainLoop();

	return 0;
}
