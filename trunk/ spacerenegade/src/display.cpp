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

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

extern PShip *playerShip; // Jam: The player's ship, duh.
extern OctTree *env;      // Jam: Collision detection of objects and the world
                          //      (environment) in general.
extern Menu *menu;        // Gum: The current menu of buttons

int screen_width = IMAGE_WIDTH;
int screen_height = IMAGE_HEIGHT;

int screenState;
bool paused;
GLfloat miniMapX = 853.0f;
GLfloat miniMapY = 174.0f;

struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;

double tacticalHudProjMat[16];  // Project matrix when the HUD is being drawn in the tactical screen.



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Color Class ------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

Color::Color() : _r(0), _g(0), _b(0), _a(1) {}

Color::Color(double r, double g, double b) : _r(r), _g(g), _b(b), _a(1) {}

Color::Color(double r, double g, double b, double a) :
	_r(r), _g(g), _b(b), _a(a) {}



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
void drawText(GLint x, GLint y, string s, Color c)
{
	// Make the nice, easy-to-use (0,0) -> (w,h) screen coords.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screen_width, 0.0, screen_height);

	// Nullify all effects of the modelview matrix.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4d(c.r(), c.g(), c.b(), c.a());
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

	// Clean up the matricies we've made.
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
	switch (screenState)
	{
		case START_SCREEN:
			displayStartScreen();
			break;
		case TACTICAL:
			if (paused)
				displayTacticalPaused();
			else
				displayTactical();
			break;
		case GAME_OVER:
			displayGameOver();
			break;
		default:
			cerr << "Got unrecognized game state! " << screenState << " Exiting!" << endl;
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

void displayTacticalPaused()
{
	drawText(512,384, "PAUSED", Color(1,1,1));
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

	if (playerShip->hlthPcnt() <= 0)
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

	drawMeters();
	drawMiniMap();

	// Shift things back into the "normal" camera that lets us look.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Set parameters back to tactical.
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void drawMeters()
{
	// Background blue-ish boxes of the meters.
	glColor3d(.3,.3,.9);
	glRecti(960, 570, 1005, 750);
	glRecti(905, 570, 950, 750);
	glRecti(850, 570, 895, 750);

	// The bar that communicates the information.
	glColor3d(.3,.8,.3);
	glRecti(965, 575, 1000, 575 + (int)(170 * playerShip->hlthPcnt()));
	glColor3d(.7,.7,.2);
	glRecti(910, 575, 945, 575 + (int)(170 * playerShip->fuelPcnt()));
	glColor3d(.8,.3,.3);
	glRecti(855, 575, 890, 575 + (int)(170 * playerShip->ammoPcnt()));

	// Draw the tick marks
	glColor3d(.9, .9, .9);
	glBegin(GL_LINES);
	glVertex2i(965, 575);
	glVertex2i(1000, 575);
	glVertex2i(1000, 745);
	glVertex2i(965, 745);
	glVertex2i(910, 575);
	glVertex2i(945, 575);
	glVertex2i(945, 745);
	glVertex2i(910, 745);
	glVertex2i(855, 575);
	glVertex2i(890, 575);
	glVertex2i(890, 745);
	glVertex2i(855, 745);
	glEnd();

	// The labels of the meters
	drawText(964, 550, "Hlth" , Color(1,1,1));
	drawText(909, 550, "Fuel" , Color(1,1,1));
	drawText(854, 550, "Ammo" , Color(1,1,1));

	// Prints the ammo left on the bottom-left of the screen.
	// This should probably be somewhere else, though ...
	stringstream stream(stringstream::in | stringstream::out);
	stream << "ammo used: " << playerShip->getAmmo();
	drawText(0,0, stream.str(), Color(1,1,1));
}

void drawMiniMap()
{
	//GLfloat cx,GLfloat cy,GLfloat r,int side
	
	//playerShip->getPos
	Object *objs[100];
	int numbObjs = 0;
	double radius = 125;

	glColor3d(.3, .3, .9);
	glCircle(miniMapX, miniMapY, 85, 20);

	//const Vec3& pos, double radius, Object **objs, int& numObjs);
	env->getArea(playerShip->getPos(), radius, objs, numbObjs);

	for(int i = 0; i < numbObjs; i++)
	{
		objs[i]->drawOnMiniMap(radius);
	}//for
}

void displayGameOver()
{
	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText(500,500 , "GAME OVER", Color(1,1,1));
	menu->draw(GL_RENDER);

	glutSwapBuffers();
}



////////////////////////////////////////////////////////////////////////////////
// ----------------- initialize different screen states --------------------- //
////////////////////////////////////////////////////////////////////////////////

void initStartScreen()
{
	// Setting this ensures all the right display
	// and input functions are called.
	screenState = START_SCREEN;

	// Set up the nice (0,0) -> (w,h) window for drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screen_width, 0, screen_height);

	// Turn off transparancies.
	glBlendFunc(GL_ONE, GL_ZERO);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Set up OpenGL for picking.
	glRenderMode(GL_SELECT);
	glInitNames();
	glRenderMode(GL_RENDER);

	// Create the appropriate menu.
	if (menu) delete menu;
	menu = new Menu(screenState);

	// Schedule a re-draw.
	glutPostRedisplay();
}

void initTactical()
{
	screenState = TACTICAL; 

	paused = false;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearIndex(0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawText(512,350 , "Loading..." , Color(1,0,0));
	glutSwapBuffers();

	// Set up the octtree, making it ready for objects to populate it.
	if (env) delete env;
	env = new OctTree(3);

	// Jam:
	// You may be asking yourself "Why not just put the call to initLeaves()
	// in the constructor?"  The answer is that the leaves need the env
	// variable to initialize themselves.  If you put the call to initLeaves()
	// in the constructor, then the new operator has not yet returned, and the
	// env variable is not yet initialized, causing a segfault when the
	// memory is accessed illegally by the leaves.  Therefore this stays here.
	env->initLeaves();

	// Populate the world with some asteroids.
	Asteroid *next;
	for (int i = 0; i < 100; i++)
	{
		Vec3 pos(rr(125,-125), rr(125,-125), rr(125,-125));
		Vec3 vel(rr(0.1,-0.1), rr(0.1,-0.1), rr(0.1,-0.1));
		next = new Asteroid(pos, vel);
		env->add(next);
	}

	// Jam:
	// Initialize the player's ship.  Don't delete it, because deleting
	// the environment should have taken care of it already.
	playerShip = new PShip();
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
	pD.farPlane    = 4000.0;

	// Puts the blending function in a way that makes for nice
	// HUD and menu transparency.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// Initialize the HUD projection matrix.  This makes for a
	// speedier change between the HUD and the rest.
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screen_width, 0, screen_height);
	glGetDoublev(GL_PROJECTION_MATRIX, tacticalHudProjMat);
	glPopMatrix();

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Color-ability.
	glEnable(GL_COLOR_MATERIAL);

	// Lights.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	adjustGlobalLighting();

	glutPostRedisplay();
}

void initGameOver()
{
	screenState = GAME_OVER;

	// Set up the perspective;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,screen_width , 0,screen_height);

	// Turn off transparancies.
	glBlendFunc(GL_ONE, GL_ZERO);

	// Clear and reset everything.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up OpenGL for picking.
	glRenderMode(GL_SELECT);
	glInitNames();
	glRenderMode(GL_RENDER);

	// Create the appropriate menu.
	if (menu) delete menu;
	menu = new Menu(GAME_OVER);

	// Schedule a re-draw.
	glutPostRedisplay();
}

