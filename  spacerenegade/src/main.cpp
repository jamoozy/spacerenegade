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
#include "globals.h"
#include "ship.h"
#include "environment.h"


// (JG)
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <stdio.h>
// /(JG)

using std::cout;
using std::endl;

#define DEBUG_MODE 1
#define PRINT_FPS 0
#define LIMIT_FPS 1

static int FPS;
static int MSPF;


int screenState;


#if (DEBUG_MODE)
	int window;
#endif

Ship *playerShip;
OctTree *env;


#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768
int width = 1024;
int height = 768;

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
} pD, hudpd;

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

// Menus and picking functions... (JG)

void drawSquares(GLenum mode)
{
	if(mode == GL_SELECT)
		glLoadName(1);
	glColor3f(0.0, 1.0, 0.0);
	glRectf(-.50, -.50, .50, .50);
	
	if(mode == GL_SELECT)
		glLoadName(2);
	glColor3f(0.0, 1.0, 0.0);
	glRectf(.60, .60, 1.1, 1.1);
	//glutSolidCube(10);
}

// End of menus and picking functions... (JG)


void doNextFrame(int value)
{
	glutPostRedisplay();
	glutTimerFunc(MSPF, doNextFrame, 0);
}

void initTactical();

void displayStartScreen()
{
	drawSquares(GL_RENDER);
	glutSwapBuffers();
	//screenState = TACTICAL;
	//initTactical();
}

void resize(int w, int h)
{
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	width = w;
	height = h;
	pD.aspect = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// This one gives nice frustum for 2D
	glFrustum(-0.1 * pD.aspect, 0.1 * pD.aspect, -0.1, 0.1, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
}

void adjustGlobalLighting()
{
	GLfloat direction[] = {1.0f,1.0f,1.0f,0.0f};
	GLfloat ambient[] = {0.2f,0.2f,0.2f};
	GLfloat diffuse[] = {0.3f,0.3f,0.3f};
	GLfloat specular[] = {0.8f,1.0f,1.0f};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}


void drawHUD()
{
	// Change the perspective so we're looking at just a boring old plane.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(hudpd.fieldOfView, hudpd.aspect, hudpd.nearPlane, hudpd.farPlane);

	// Change the matrix to modelview and translate all things to the
	// one and only one plane we can look at.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0,0,-50);

	// Disable the lighting and draw the HUD here.
	glDisable(GL_LIGHTING);  // Now it won't look like the HUD is part of the world.
	glColor4d(0,0,1,0.2);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRecti(0,0 , 10,10);
	glRecti(10,10 , 20,20);
	glRecti(10,-10 , 20,0);
	glRecti(-10,10 , 0,20);
	glRecti(0,0 , -10,-10);
	glRecti(-10,-10 , -20,-20);
	glRecti(-10,10 , -20,0);
	glRecti(10,-10 , 0,-20);

	// Shift things back into the "normal" camera that lets us look.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// Turn the lighting back on.
	glEnable(GL_LIGHTING);
}

void displayTactical(void)
{
	#if (DEBUG_MODE)
		glutSetWindow(window);
	#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	adjustCamera();

	adjustGlobalLighting();

	handleInput();

	#if (PRINT_FPS)
		if (last_time != time(NULL))
		{
			std::cout << ctime(&last_time) << "fps: " << frames_this_second << std::endl;
			last_time = time(NULL);
			frames_this_second = 0;
		}
		else
			frames_this_second++;
	#endif

	env->update();

	drawHUD();

	glutSwapBuffers();
}

void display()
{
	if (screenState == START_SCREEN)
	{
		displayStartScreen();
	}
	else if (screenState == TACTICAL)
	{
		displayTactical();
	}
}

//##########################################
// Init display settings

void initStartScreen()
{
	
	pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 200.0;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glTranslatef( 0,0,-5);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void initTactical()
{
	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = FPS;
	#endif

	// Perspective projection parameters
	pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 2000.0;

	hudpd.fieldOfView = 45.0;
	hudpd.aspect = (float)width/height;
	hudpd.nearPlane = 49.9;
	hudpd.farPlane = 50.1;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearIndex(0);
	glClearDepth(1);

	// Color-ability.
	glEnable(GL_COLOR_MATERIAL);

	// Lights.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	adjustGlobalLighting();
}

//##########################################
// Main function

double rr(double max, double min)
{
	return (rand() * (max - min)) / RAND_MAX + min;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	FPS = 0;
	MSPF = 17;

	srand(time(NULL));

	env = new OctTree();
	env->initLeaves();

	Asteroid *next;
	for (int i = 0; i < 1000; i++)
	{
		Vec3 pos(rr(1000,-1000), rr(1000,-1000), rr(1000,-1000));
		Vec3 vel(rr(0.1,-0.1), rr(0.1,-0.1), rr(0.1,-0.1));
		next = new Asteroid(pos, vel);
		env->add(next);
	}

	playerShip = new Ship();
	playerShip->setAt(0,0,0);
	env->add(playerShip);

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
	
	screenState = START_SCREEN;
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
