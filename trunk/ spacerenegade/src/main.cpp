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

extern void mouseClick(int, int, int, int);

enum { START_SCREEN, TACTICAL };
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

class PerspectiveData
{
private:
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;

public:
	//Constructors
	PerspectiveData():fieldOfView(0), aspect(0), nearPlane(0), farPlane(0){}
	PerspectiveData(float fieldOfView, float aspect, float nearPlane, float farPlane)
		:fieldOfView(fieldOfView), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane){}

	//Getter Functions
	float getFieldOfView(){return fieldOfView;}
	float getAspect(){return aspect;}
	float getNearPlane(){return nearPlane;}
	float getFarPlane(){return farPlane;}

	//Setter Functions
	void setFieldOfView(float fieldOfView){fieldOfView = fieldOfView;}
	void setAspect(float aspect){aspect = aspect;}
	void setNearPlane(float nearPlane){nearPlane = nearPlane;}
	void setFarPlane(float farPlane){farPlane = farPlane;}
	
};

PerspectiveData pD;
/*
struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;
*/

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
	pD.setAspect((float)w/(float)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// This one gives nice frustum for 2D
	glFrustum(-0.1 * pD.getAspect(), 0.1 * pD.getAspect(), -0.1, 0.1, 0.1, 20.0);
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

	drawSquares(GL_RENDER);///////////////////////////////
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
	
	// Perspective projection parameters
	//pD = new PerspectiveData(45.0, (float)IMAGE_WIDTH/IMAGE_HEIGHT, 0.1,200.0);
	/*pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 200.0;
	*/
	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.getFieldOfView(), pD.getAspect(), pD.getNearPlane(), pD.getFarPlane());

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);
	
	if(Keyboard::getKeyboard()->isDown(SR_KEY_S)){
		screenState = TACTICAL;
		initTactical();
	}//if
	else{
		//glMatrixMode(GL_PROJECTION);
		glTranslatef( 0,0,-5);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		//gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}//else
}

void initTactical()
{
	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = FPS;
	#endif

	// Perspective projection parameters
	//pD.fieldOfView = 45.0;
	//pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	//pD.nearPlane   = 0.1;
	pD.setFarPlane(2000.0);

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.getFieldOfView(), pD.getAspect(), pD.getNearPlane(), pD.getFarPlane());

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

	pD = PerspectiveData(45.0, (float)IMAGE_WIDTH/IMAGE_HEIGHT, 0.1, 200.0);
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
	
	glutMouseFunc(mouseClick);
	
    glutMainLoop();
	

	return 0;
}
