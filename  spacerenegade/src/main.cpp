#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
//#include <signal.h>
#ifdef WIN32
#include <windows.h>
#endif
//#ifdef __APPLE__
//#include <OpenGL/gl.h>
//#include <GLUT/glut.h>
//#else
#include <GL/gl.h>
#include <GL/glut.h>
//#endif
//#include "viewModule.h"
#include "input.h"
//#include "cube.h"
//#include "bouncer.h"
//#include "geometry.h"

int window;
//int updateFlag;

//bool running_mode = kay;

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;

void cleanup(int sig)
{
	// insert cleanup code here (i.e. deleting structures or so)
	//fprintf(stderr,"Cleaning up\n");

	exit(0);
}


//##########################################
// OpenGL Display function

void display(void)
{
	glutSetWindow(window);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}


//##########################################
// Init display settings

void initDisplay()
{
	// Perspective projection parameters
	pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane = 50.0;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearIndex(0);
	//glClearDepth(1);

	// setup lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black_color);
	//glLightfv(GL_LIGHT0, GL_POSITION, initial_light_pos);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
}

//##########################################
// Main function

int main(int argc, char **argv)
{

	//signal(SIGHUP, cleanup);

	glutInit(&argc, argv);

	srand(time(NULL));

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT);

	window = glutCreateWindow("Space Renegade");

	glutDisplayFunc(display);
	glutKeyboardFunc(readKeyboard);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutIdleFunc(display);

	initDisplay();

	glutMainLoop();

	return 0;
}
