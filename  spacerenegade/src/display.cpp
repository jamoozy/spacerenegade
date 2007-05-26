#include <iostream>
#include <string>
#include <cmath>
#include "GL/glut.h"
#include "display.h"
#include "ship.h"
#include "environment.h"
#include "button.h"
#include "globals.h"
#include "input.h"
#include "camera.h"
#include "asteroid.h"
#include "menu.h"

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

extern Ship *playerShip;  // Jam: The player's ship, duh.
extern OctTree *env;      // Jam: Collision detection of objects and the world
                          //      (environment) in general.
extern Menu *menu;        // Gum: The current menu of buttons

int screen_width = IMAGE_WIDTH;
int screen_height = IMAGE_HEIGHT;

int screenState;
GLfloat miniMapX = 20;
GLfloat miniMapY = -11.5;

struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD, hudpd;

double tacticalHudProjMat[16];  // Project matrix when the HUD is being drawn in the tactical screen.



////////////////////////////////////////////////////////////////////////////////
// ------------------ Util func's (maybe move later?) ----------------------- //
////////////////////////////////////////////////////////////////////////////////

// Jam:
// "rr" stands for "Random Range".  It's a nice little function meant
// (initially) for creating random-ish asteroids.
double rr(double max, double min)
{
	return (rand() * (max - min)) / RAND_MAX + min;
}

// Draw some text somewhere.
void drawText(GLint x, GLint y, string s, GLfloat r, GLfloat g, GLfloat b)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
			0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r,g,b);
	x = (int)floor(x + (.5 * screen_width));
	y = (int)floor(y + (.5 * screen_height));
	glRasterPos2i(x, y);
	for(unsigned int i = 0, lines = 0; i < s.size(); i++)
	{
		if (s.at(i) == '\n')
		{
			lines++;
			glRasterPos2i(x, y-(lines*18));
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s.at(i));
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void glCircle(GLfloat cx, GLfloat cy, GLfloat r, int side)
{
 
    glBegin(GL_POLYGON);
    for (int i = 0; i < side; i++)
	{
		glVertex2f(cx+r*cos(i*2*M_PI/side),cy+r*sin(i*2*M_PI/side));
    }

    glEnd();
}

void resize(int w, int h)
{
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	screen_width = w;
	screen_height = h;
	pD.aspect = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// This one gives nice frustum for 2D
	glFrustum(-0.1 * pD.aspect, 0.1 * pD.aspect, -0.1, 0.1, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
}



////////////////////////////////////////////////////////////////////////////////
// ------------------ display different screen states ----------------------- //
////////////////////////////////////////////////////////////////////////////////

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
	else if (screenState == GAME_OVER)
	{
		displayGameOver();
	}
	else
	{
		std::cerr << "Got unrecognized game state! " << screenState << " Exiting!" << std::endl;
	}
}

// Displays the start screen.
void displayStartScreen()
{
	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	menu->draw(GL_RENDER);

	glutSwapBuffers();
}

void displayTactical()
{
	#if (DEBUG_MODE)
		glutSetWindow(window);
	#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	adjustCamera();

	adjustGlobalLighting();
	
	handleTacticalInput();

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

	if (playerShip->getHealth() <= 0)
		initGameOver();
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
	glPushMatrix();
	glLoadMatrixd(tacticalHudProjMat);

	// Change the matrix to modelview so it can be "cleared".
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable transparency, disable the lighting and deptch checking,
	// and draw the HUD here.
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);  // Now it won't look like the HUD is part of the world.

	// The health meter.
	glColor3d(.3,.3,.8);
	glRecti(24,10,27,20);
	glColor3d(.8,.3,.3);
	glRectd(24.2,10.2,26.8,10.2 + playerShip->getHealth() * 9.6);

	glColor4d(0,0,1,0.2);
	glRecti(-30,-5,30,-20);
	drawMiniMap();

	// Shift things back into the "normal" camera that lets us look.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Set parameters back to tactical.
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void drawMiniMap()
{	//GLfloat cx,GLfloat cy,GLfloat r,int side
	
	
	//playerShip->getPos
	Object *objs[100];
	int numbObjs = 0;
	double radius = 125;

	glCircle(miniMapX, miniMapY, 5, 20);

	//const Vec3& pos, double radius, Object **objs, int& numObjs);
	env->getArea(playerShip->getPos(), radius, objs, numbObjs);

	for(int i = 0; i < numbObjs; i++)
	{
		objs[i]->drawOnMiniMap(radius);
	}//for

	//exit(0);
}

void displayGameOver()
{
	cleanup();
}



////////////////////////////////////////////////////////////////////////////////
// ----------------- initialize different screen states --------------------- //
////////////////////////////////////////////////////////////////////////////////

void initStartScreen()
{
	screenState = START_SCREEN;
	
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

	glRenderMode(GL_SELECT);
	glInitNames();
	glRenderMode(GL_RENDER);

	if (menu) delete menu;
	menu = new Menu(screenState);
}

void initTactical()
{
	screenState = TACTICAL; 

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearIndex(0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Button b("Loading...",0.0 , 0.0f,0.0f , 0.0f,0.0f,0.0f , 0,NULL);
	b.Place(GL_RENDER);

	glutSwapBuffers();

	// Set up the octtree, making it ready for objects to populate it.
	if (env) delete env;
	env = new OctTree();
	env->initLeaves();

	// Populate the world with some asteroids.
	Asteroid *next;
	for (int i = 0; i < 100; i++)
	{
		Vec3 pos(rr(250,-250), rr(250,-250), rr(250,-250));
		Vec3 vel(rr(0.1,-0.1), rr(0.1,-0.1), rr(0.1,-0.1));
		next = new Asteroid(pos, vel);
		env->add(next);
	}

	// Jam:
	// Initialize the player's ship.  Don't delete it, because deleting
	// the environment should have taken care of it already.
	playerShip = new Ship();
	playerShip->setAt(0,0,0);
	env->add(playerShip);

	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = FPS;
	#endif

	// Perspective projection parameters
	pD.fieldOfView = 45.0;
	pD.aspect      = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
	pD.nearPlane   = 0.1;
	pD.farPlane    = 2000.0;

	// Special perspective for menu items.
	hudpd.fieldOfView = 45.0;
	hudpd.aspect = (float)screen_width/screen_height;
	hudpd.nearPlane = 49.9;
	hudpd.farPlane = 50.1;

	// Puts the blending function in a way that makes for nice
	// HUD and menu transparency.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// Initialize the HUD projection matrix.
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(hudpd.fieldOfView, hudpd.aspect, hudpd.nearPlane, hudpd.farPlane);
	glTranslated(0,0,-50);
	glGetDoublev(GL_PROJECTION_MATRIX, tacticalHudProjMat);
	glPopMatrix();

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Color-ability.
	glEnable(GL_COLOR_MATERIAL);

	// Lights.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	adjustGlobalLighting();
}

void initGameOver()
{
	screenState = GAME_OVER;
}

