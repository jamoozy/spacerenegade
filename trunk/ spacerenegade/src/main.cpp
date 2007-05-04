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
#include <GL/gl.h>
#include <GL/glut.h>
#include "input.h"
#include "asteroid.h"

int window;

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600

#if (PRINT_FPS)
time_t last_time;
int frames_this_second;
#endif

Asteroid *ast;

struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;

void cleanup(int sig)
{
	delete [] ast;
	exit(0);
}


//##########################################
// OpenGL Display function

void display(void)
{
	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if (PRINT_FPS)
	if (last_time != time(NULL))
	{
		std::cout << ctime(&last_time) << "\b\b: " << frames_this_second << std::endl;
		last_time = time(NULL);
		frames_this_second = 0;
	}
	else
		frames_this_second++;
#endif


	for (int i = 0; i < 6; i++)
		ast[i].draw();

	glutSwapBuffers();
}


//##########################################
// Init display settings

void initDisplay()
{
#if (PRINT_FPS)
	last_time = 0;
	frames_this_second = 0;
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

	ast = new Asteroid[6];
	
	ast[0] = Asteroid( 0, 0,-9 ,  0.00,0.00,-0.01);
	ast[1] = Asteroid( 0, 0, 9 ,  0.00,0.00,-0.01);
	ast[2] = Asteroid( 0,-9, 0 ,  0.00,0.00,-0.01);
	ast[3] = Asteroid( 0, 9, 0 ,  0.00,0.00,-0.01);
	ast[4] = Asteroid(-9, 0, 0 ,  0.00,0.00,-0.01);
	ast[5] = Asteroid( 9, 0, 0 ,  0.00,0.00,-0.01);

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
