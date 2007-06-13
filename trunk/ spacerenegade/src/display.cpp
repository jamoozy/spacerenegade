#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <AL/alut.h>
#include "display.h"
#include "ship.h"
#include "environment.h"
#include "button.h"
#include "globals.h"
#include "input.h"
#include "camera.h"
#include "asteroid.h"
#include "planet.h"
#include "menu.h"
#include "mission.h"
#include "factionInfo.h"

#include "objective.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::vector;
using std::string;

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

#if (DEBUG_MODE)
	extern int window;
#endif
#if (PRINT_FPS)
	extern int FPS;
	extern int MSPF;
	time_t last_time;
	int frames_this_second;
	int frames_last_second;
#endif

extern PShip *playerShip; // Jam: The player's ship, duh.
extern Planet *planet;    // Jam: The planet to land on.
extern OctTree *env;      // Jam: Collision detection of objects and the world
                          //      (environment) in general.
extern Menu *menu;        // Gum: The current menu of buttons
extern SoundFactory *soundFactory;
extern vector<Mission*> missionsAvailable;
extern vector<Mission*> missionsOn;

vector<Ship*> enemyShips;

FactionInfo *playerFactionInfo, *redFactionInfo, *blueFactionInfo,
	*whiteFactionInfo, *otherFactionInfo;

int screen_width = IMAGE_WIDTH;
int screen_height = IMAGE_HEIGHT;

int screenState;
bool paused;
int zoom = 1;
GLfloat miniMapX = 853.0f;
GLfloat miniMapY = 174.0f;

// Sound names.
string soundNames[7] = {"gunshot","hit","explosion-asteroid","explosion-ship","thrust","missionaccepted","heal"};
Sound ambientMusic;


struct perspectiveData 
{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
} pD;

double tacticalHudProjMat[16];  // Project matrix when the HUD is being drawn in the tactical screen.


////////////////////////////////////////////////////////////////////////////////
// ------------------------- Sound Class ------------------------------------ //
////////////////////////////////////////////////////////////////////////////////

Sound::Sound(string name) : name(name)
{
	string directory = "art/" + name + ".wav";
	// Load the sound.
	if ((buffer = alutCreateBufferFromFile(directory.c_str())) == AL_NONE) {
		cout << "ALUT Error for file " << directory << ": "
			<< alutGetErrorString(alutGetError()) << endl;
	} else {
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, buffer);
	}
}

void Sound::play()
{
	if (buffer != AL_NONE)
		alSourcePlay(source);
}

void Sound::stop()
{
	if (buffer != AL_NONE)
		alSourceStop(source);
}

void Sound::setLooping()
{
	alSourcei(source, AL_LOOPING, AL_TRUE);
}

bool Sound::operator ==(const string &soundName)
{
	return (name.compare(soundName) == 0);
}

void Sound::shiftMusic(string newName)
{
	stop();
	string directory = "art/" + newName + ".wav";
	// Load the sound.
	if ((buffer = alutCreateBufferFromFile(directory.c_str())) == AL_NONE) {
		cout << "ALUT Error for file " << directory << ": "
			<< alutGetErrorString(alutGetError()) << endl;
	} else {
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, buffer);
	}
	play();
	setLooping();
	
}


////////////////////////////////////////////////////////////////////////////////
// --------------------- SoundFactory Class --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

SoundFactory::SoundFactory(string *names)
{

	for(unsigned int i = 0; i < names->size(); i++)
		sounds.push_back(Sound(names[i]));
	
}

void SoundFactory::play(const string &name)
{
	for (unsigned int i = 0; i < sounds.size(); i++)
	{
		if(sounds[i] == name)
		{
			sounds[i].play();
			return;
		}
	}
}

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
void drawText(GLint x, GLint y, string s, Color c, bool center)
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
	if (center)
	{
		x -= (GLint)(s.length() * 4.5);
		y += 7;
	}

	glRasterPos2i(x, y);
	for(unsigned int i = 0, lines = 0; i < s.size(); i++)
	{
		if (s.at(i) == '\n')
		{
			lines++;
			glRasterPos2i(x, y-(lines*18));
		}
		else
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

void drawBox(int x1, int y1, int x2, int y2, double r, double g, double b) // (Gum)
{
	glColor3d(r,g,b);
	glRecti(x1, y1, x2, y2);
	glColor3d(0,0,0);
	glRecti(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
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
			if (paused) {
				displayTacticalPaused();
			} else {
				displayTactical();
			} break;
		case PLANET:
			displayPlanet();
			break;
		case MISSION_BOARD:
			displayMissionBoard();
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
	glDisable(GL_LIGHTING);
	drawText(512,384, "PAUSED", Color(1,1,1), true);
	glutSwapBuffers();
	glEnable(GL_LIGHTING);
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

	env->update();
	glDisable(GL_BLEND);
	env->draw(1);        // 1st pass.
	glEnable(GL_BLEND);  // Enable transparency.
	env->draw(2);        // 2nd (transparency) pass.

	drawHUD();         // Keep on transparency for the HUD.
	drawObjectives();

	glutSwapBuffers();

	if (playerShip->hlthPcnt() <= 0) {
		//soundFactory.
		//if (boomBuffer != AL_NONE)
		//	alSourcePlay(boomSource);
		initGameOver();
	}

	if (playerShip->collidesWith(planet))
	{
		// Jam:
		// Put it here so that all the other state changes to openGL do
		// not effect the state that the displayPlanet expects.
		initPlanet();
	}
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
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);  // Now it won't look like the HUD is part of the world.
	glDisable(GL_TEXTURE_2D);  // Gives us full-range colors for some reason.

	drawMeters();
	drawMiniMap();

	// Shift things back into the "normal" camera that lets us look.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Set parameters back to tactical.
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void drawObjectives()
{
	// for debugging displaying of titles/missions, try replacing missionsOn with missionsAvailable
	int height = 700;
	for (int i = 0; i < (int)missionsOn.size(); i++)
	{
		Mission *m = missionsOn.at(i);
		// Check to see if mission has been completed.
		
		drawText(10, height, m->getTitle() , Color(1,1,1), false);
		for (int k = 0; k < m->getNumObjs(); k++)
		{
			height -= 30;
			Objective *o = m->getObjective(k);
			string des = o->getDescription();
			drawText(20, height, des, Color(.5,.5,.5), false);
		}
		height -= 40;
	}
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
	glColor3d(.7,.7,.1);
	glRecti(800, 575, 835, 575 + (int)(170 * playerShip->shldPcnt()));

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
	drawText(964, 550, "Hlth" , Color(1,1,1), false);
	drawText(909, 550, "Fuel" , Color(1,1,1), false);
	drawText(854, 550, "Ammo" , Color(1,1,1), false);
	drawText(799, 550, "Shld" , Color(1,1,1), false);

	// Prints the ammo left on the bottom-left of the screen.
	// This should probably be somewhere else, though ...
	stringstream hstream(stringstream::in | stringstream::out);
	hstream << "hlth left: " << playerShip->getHlth();
	stringstream fstream(stringstream::in | stringstream::out);
	fstream << "fuel left: " << playerShip->fuelPcnt();
	stringstream astream(stringstream::in | stringstream::out);
	astream << "ammo left: " << playerShip->getAmmo();
	stringstream sstream(stringstream::in | stringstream::out);
	sstream << "shld left: " << playerShip->shldPcnt();

	drawText(0,45, hstream.str(), Color(1,1,1), false);
	drawText(0,30, fstream.str(), Color(1,1,1), false);
	drawText(0,15, astream.str(), Color(1,1,1), false);
	drawText(0, 0, sstream.str(), Color(1,1,1), false);

	#if (PRINT_FPS)
		if (last_time != time(NULL))
		{
			last_time = time(NULL);
			frames_last_second = frames_this_second;
			frames_this_second = 0;
		}
		else
		{
			frames_this_second++;
		}
		stringstream debug(stringstream::in | stringstream::out);
		debug << ctime(&last_time) << "fps: " << frames_last_second;
		drawText(800,30 , debug.str() , Color(1,1,1), false);
	#endif
}

void drawMiniMap()
{
	vector<Object*> objs;
	double num = 0;


	glColor3d(.3, .3, .9);
	glCircle(miniMapX, miniMapY, 85, 20);

	if(zoom == 0)
		num = 75*75+75*75+75*75;
	else if(zoom == 2)
		num = 100*100+100*100+100*100;
	else if(zoom == 1)
		num = 125*125+125*125+125*125;
	else
		drawText(500, 100, "Zoom out of range!", Color(1,.3,.3), false);

	double radius = sqrt(num);

	env->getArea(playerShip->getPos(), radius, objs);
	for(unsigned int i = 0; i < objs.size(); i++)
	{
		objs[i]->drawOnMiniMap(radius);
	}//for
}

void displayMissionBoard()
{
	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBox(10, 400, 400, 750, .4,.8,.1);
	drawBox(500, 400, 900, 750, .4,.8,.1);

	if (menu->needsRefresh())
	{
		delete menu;
		menu = new Menu(screenState);
	}
	menu->draw(GL_RENDER);

	glutSwapBuffers();
}

void pauseMenu() // (Gum) Called when in a menu
{
	playerShip->stabilize();
	glutSwapBuffers();
}

void displayPlanet()
{
	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	std::stringstream ss1;
	std::stringstream ss2;
	std::stringstream ss3;
	std::stringstream ss4;
	std::stringstream ss1a;
	std::stringstream ss2a;
	std::stringstream ss3a;
	std::string str;

	ss1 << 100 * playerShip->fuelPcnt();
	ss1 >> str;
	drawText(600, 420, "Fuel:   " + str + "% remaining", Color(.4, 1, .4), false);
	ss1a << playerShip->costToRefuel();
	ss1a >> str;
	drawText(600, 400, "Costs " + str + " credits to fully refuel", Color(.4, 1, .4), false);

	ss2 << playerShip->getAmmo();
	ss2 >> str;
	drawText(600, 370, "Ammo:   " + str + " bullets", Color(.4, 1, .4), false);
	ss2a << playerShip->costToReload();
	ss2a >> str;
	drawText(600, 350, "Costs " + str + " credits to fully reload", Color(.4, 1, .4), false);

	ss3 << 100 * playerShip->hlthPcnt();
	ss3 >> str;
	drawText(600, 320, "Health: " + str + "% armor remaining", Color(.4, 1, .4), false);
	ss3a << playerShip->costToHeal();
	ss3a >> str;
	drawText(600, 300, "Costs " + str + " credits to fully heal", Color(.4, 1, .4), false);

	ss4 << playerShip->getCredits();
	ss4 >> str;
	drawText(600, 250, "Money:  " + str + " credits", Color(.4, .4, 1), false);

	menu->draw(GL_RENDER);

	glutSwapBuffers();
}

void displayGameOver()
{
	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText(500,500 , "GAME OVER", Color(1,1,1), false);
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
	
	ambientMusic.shiftMusic("music/01");

	// Set up the nice (0,0) -> (w,h) window for drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screen_width, 0, screen_height);

	// Turn off lighting
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

// (Gum)
// initNewGame sets up parameters for ship position/stats,
// while initTactical sets up just graphics, to be used after leaving a planet
void initNewGame()
{
	//screenState = TACTICAL; 
	soundFactory = new SoundFactory(soundNames);

	paused = false;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText(512,384 , "Loading..." , Color(1,0,0), true);
	glutSwapBuffers();

	// Set up the octtree, making it ready for objects to populate it.
	if (env) delete env;
	env = new OctTree(3);

	// Set Up Faction Information
	vector<Faction> friendly;
	vector<Faction> neutral;
	vector<Faction> enemy;

	// Player Faction
	friendly.push_back(PLAYER);
	neutral.push_back(WHITE);
	neutral.push_back(OTHER);
	enemy.push_back(RED);
	enemy.push_back(BLUE);
	playerFactionInfo = new FactionInfo(PLAYER, friendly, neutral, enemy);
	friendly.clear();
	neutral.clear();
	enemy.clear();

	// Red Faction
	friendly.push_back(RED);
	neutral.push_back(WHITE);
	neutral.push_back(OTHER);
	neutral.push_back(PLAYER);
	neutral.push_back(BLUE);
	redFactionInfo = new FactionInfo(RED, friendly, neutral, enemy);
	friendly.clear();
	neutral.clear();
	enemy.clear();

	// Blue Faction
	friendly.push_back(BLUE);
	neutral.push_back(WHITE);
	neutral.push_back(OTHER);
	neutral.push_back(PLAYER);
	neutral.push_back(RED);
	blueFactionInfo = new FactionInfo(BLUE, friendly, neutral, enemy);
	friendly.clear();
	neutral.clear();
	enemy.clear();

	// White Faction
	friendly.push_back(WHITE);
	neutral.push_back(RED);
	neutral.push_back(BLUE);
	neutral.push_back(PLAYER);
	neutral.push_back(OTHER);
	whiteFactionInfo = new FactionInfo(WHITE, friendly, neutral, enemy);
	friendly.clear();
	neutral.clear();
	enemy.clear();

	// Other Faction
	neutral.push_back(PLAYER);
	neutral.push_back(RED);
	neutral.push_back(BLUE);
	neutral.push_back(WHITE);
	neutral.push_back(OTHER);
	otherFactionInfo = new FactionInfo(OTHER, friendly, neutral, enemy);
	friendly.clear();
	neutral.clear();
	enemy.clear();

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
	for (int i = 0; i < 50; i++)
	{
		Vec3 pos(rr(125,-125), rr(125,-125), rr(125,-125));//-20,0,250-(20*i));//
		Vec3 vel(rr(0.1,-0.1), rr(0.1,-0.1), rr(0.1,-0.1));//0,0,-0.1);//
		next = new Asteroid(5, pos, vel);
		env->add(next);
	}

	// (Gum)
	// Placing one planet in the environment
	Vec3 pos (0, 0, 100); // Random or static position?
	planet = new Planet(pos, 25); // radius too big? too small?
	env->add(planet);

	// (Gum)
	// Creating list of available missions
	missionsOn.clear();
	missionsAvailable.clear();
	missionsAvailable.push_back(new Mission(0));
	missionsAvailable.push_back(new Mission(1));

	// Jam:
	// Initialize the player's ship.  Don't delete it, because deleting
	// the environment should have taken care of it already.
	playerShip = new PShip(new Blaster(), new BasicHull(), new BasicShield());
	playerShip->setAt(0,0,0);
	env->add(playerShip);

	// PM:
	// Initialize the enemy ship.  This will be removed once missions are implemented
	Ship* enemy1;
	enemy1 = new BasicBlueShip(new Blaster(), new BasicHull(), new BasicShield());
	enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
	env->add(enemy1);
	enemyShips.push_back(enemy1);
	enemy1 = new BasicBlueShip(new Blaster(), new BasicHull(), new BasicShield());
	enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
	env->add(enemy1);
	enemyShips.push_back(enemy1);
	enemy1 = new BasicRedShip(new Blaster(), new BasicHull(), new BasicShield());
	enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
	env->add(enemy1);
	enemyShips.push_back(enemy1);
	enemy1 = new BasicRedShip(new Blaster(), new BasicHull(), new BasicShield());
	enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
	env->add(enemy1);
	enemyShips.push_back(enemy1);

	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = frames_last_second = FPS;
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

	initTactical();
}

void initTactical()
{
	bool leavingPlanet = (screenState == PLANET);
	screenState = TACTICAL; 

	playerShip->stop();
	
	ambientMusic.shiftMusic("music/04");

	paused = false;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText(512,384 , "Loading..." , Color(1,0,0), true);
	glutSwapBuffers();

	if (leavingPlanet)
	{
		playerShip->setAt(0,0,50);
	}
	//env->add(playerShip);

	#if (PRINT_FPS)
		last_time = 0;
		frames_this_second = frames_last_second = FPS;
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

void initMissionBoard() //(Gum)
{
	pauseMenu();

	Keyboard::reset();

	// Setting this ensures all the right display
	// and input functions are called.
	screenState = MISSION_BOARD;

	// Set up the nice (0,0) -> (w,h) window for drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screen_width, 0, screen_height);

	// Restore compatible GL state.
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

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

void initPlanet()
{
	screenState = PLANET;

	ambientMusic.shiftMusic("music/01");

	// Set up the nice (0,0) -> (w,h) window for drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,screen_width , 0,screen_height, -1.0, 1.0);

	// Restore compatible GL state.
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

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

void initGameOver()
{
	screenState = GAME_OVER;

	ambientMusic.shiftMusic("music/01");

	// Set up the perspective;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,screen_width , 0,screen_height, -1.0, 1.0);

	// Restore a compatible GL state.
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

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


////////////////////////////////////////////////////////////////////////////////
// --------------------------- Update the player ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

void refuelPlayer()
{
	//if (playerShip->canAfford(playerShip->costToRefuel()))
		playerShip->refuel();
}

void reloadPlayerAmmo()
{
	//if (playerShip->canAfford(playerShip->costToReload()))
		playerShip->reload();
}

void healPlayer()
{
	//if (playerShip->canAfford(playerShip->costToHeal()))
	//{
		soundFactory->play("heal");
		playerShip->heal();
	//}
}

