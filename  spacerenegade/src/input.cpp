#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "environment.h"
#include "globals.h"
#include "camera.h"
#include "input.h"
#include "ship.h"
#include "button.h"
#include "display.h"
#include "menu.h"

using std::cout;
using std::cerr;
using std::endl;

extern Menu *menu;
extern int screenState;
extern PShip *playerShip;
extern OctTree *env;
extern int screen_width;
extern int screen_height;
extern bool paused;
extern int zoom;
extern vector<Ship*> enemyShips;

// Aliases for all of the controls.
int SR_PITCH_BACK_1, SR_PITCH_BACK_2, SR_PITCH_BACK_3,
    SR_PITCH_FORWARD_1, SR_PITCH_FORWARD_2, SR_PITCH_FORWARD_3, 
    SR_YAW_RIGHT_1, SR_YAW_RIGHT_2, SR_YAW_RIGHT_3,
    SR_YAW_LEFT_1, SR_YAW_LEFT_2, SR_YAW_LEFT_3,
    SR_ROLL_LEFT_1, SR_ROLL_LEFT_2, SR_ROLL_LEFT_3,
    SR_ROLL_RIGHT_1, SR_ROLL_RIGHT_2, SR_ROLL_RIGHT_3,
    SR_ACCELERATE_1, SR_ACCELERATE_2, SR_ACCELERATE_3,
    SR_DECELERATE_1, SR_DECELERATE_2, SR_DECELERATE_3,
    SR_BREAK_1, SR_BREAK_2, SR_BREAK_3,
    SR_FIRE_PRIMARY_1, SR_FIRE_PRIMARY_2, SR_FIRE_PRIMARY_3,
    SR_FIRE_SECONDARY_1, SR_FIRE_SECONDARY_2, SR_FIRE_SECONDARY_3,
    SR_FIRE_TERTIARY_1, SR_FIRE_TERTIARY_2, SR_FIRE_TERTIARY_3,
    SR_ZOOM_IN_1, SR_ZOOM_IN_2, SR_ZOOM_IN_3,
    SR_ZOOM_OUT_1, SR_ZOOM_OUT_2, SR_ZOOM_OUT_3,
    SR_MINIMAP_ZOOM_IN_1, SR_MINIMAP_ZOOM_IN_2, SR_MINIMAP_ZOOM_IN_3,
    SR_MINIMAP_ZOOM_OUT_1, SR_MINIMAP_ZOOM_OUT_2, SR_MINIMAP_ZOOM_OUT_3,
    SR_TRACTOR_BEAM_TOGGLE_1, SR_TRACTOR_BEAM_TOGGLE_2, SR_TRACTOR_BEAM_TOGGLE_3;



////////////////////////////////////////////////////////////////////////////////
// -------------------------- Keyboard Definitions -------------------------- //
////////////////////////////////////////////////////////////////////////////////

Keyboard *Keyboard::keyboard = NULL;

Keyboard::Keyboard() : keys(new bool[SR_NUM_KEYS])
{
	for (int i = 0; i < SR_NUM_KEYS; i++)
		keys[i] = false;
}

void Keyboard::reset()
{
	for (int i = 0; i < SR_NUM_KEYS; i++)
		keyboard->keys[i] = false;
}

Keyboard *Keyboard::getKeyboard()
{
	if (keyboard == NULL)
		keyboard = new Keyboard();

	return keyboard;
}

void Keyboard::cleanUp()
{
	if (keyboard != NULL)
	{
		delete keyboard;
		keyboard = NULL;
	}
}

void initKeyboardLayout()
{
	SR_PITCH_BACK_1 = SR_KEY_D;
	SR_PITCH_BACK_2 = SR_KEY_DOWN;
	SR_PITCH_BACK_3 = -1;
	SR_PITCH_FORWARD_1 = SR_KEY_E;
	SR_PITCH_FORWARD_2 = SR_KEY_UP;
	SR_PITCH_FORWARD_3 = -1;
	SR_YAW_RIGHT_1 = SR_KEY_F;
	SR_YAW_RIGHT_2 = SR_KEY_RIGHT;
	SR_YAW_RIGHT_3 = -1;
	SR_YAW_LEFT_1 = SR_KEY_S;
	SR_YAW_LEFT_2 = SR_KEY_LEFT;
	SR_YAW_LEFT_3 = -1;
	SR_ROLL_LEFT_1 = SR_KEY_W;
	SR_ROLL_LEFT_2 = -1;
	SR_ROLL_LEFT_3 = -1;
	SR_ROLL_RIGHT_1 = SR_KEY_R;
	SR_ROLL_RIGHT_2 = -1;
	SR_ROLL_RIGHT_3 = -1;
	SR_ACCELERATE_1 = SR_KEY_SPACE;
	SR_ACCELERATE_2 = -1;
	SR_ACCELERATE_3 = -1;
	SR_DECELERATE_1 = SR_KEY_A;
	SR_DECELERATE_2 = -1;
	SR_DECELERATE_3 = -1;
	SR_BREAK_1 = SR_KEY_G;
	SR_BREAK_2 = -1;
	SR_BREAK_3 = -1;
	SR_FIRE_PRIMARY_1 = SR_KEY_ENTER;
	SR_FIRE_PRIMARY_2 = -1;
	SR_FIRE_PRIMARY_3 = -1;
	SR_FIRE_SECONDARY_1 = -1;
	SR_FIRE_SECONDARY_2 = -1;
	SR_FIRE_SECONDARY_3 = -1;
	SR_FIRE_TERTIARY_1 = -1;
	SR_FIRE_TERTIARY_2 = -1;
	SR_FIRE_TERTIARY_3 = -1;
	SR_ZOOM_IN_1 = -1;
	SR_ZOOM_IN_2 = -1;
	SR_ZOOM_IN_3 = -1;
	SR_ZOOM_OUT_1 = -1;
	SR_ZOOM_OUT_2 = -1;
	SR_ZOOM_OUT_3 = -1;
	SR_MINIMAP_ZOOM_IN_1 = SR_KEY_Z;
	SR_MINIMAP_ZOOM_IN_2 = -1;
	SR_MINIMAP_ZOOM_IN_3 = -1;
	SR_MINIMAP_ZOOM_OUT_1 = -1;
	SR_MINIMAP_ZOOM_OUT_2 = -1;
	SR_MINIMAP_ZOOM_OUT_3 = -1;
	SR_TRACTOR_BEAM_TOGGLE_1 = SR_KEY_X;
	SR_TRACTOR_BEAM_TOGGLE_2 = -1;
	SR_TRACTOR_BEAM_TOGGLE_3 = -1;
}



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- Mouse Definitions --------------------------- //
////////////////////////////////////////////////////////////////////////////////

Mouse *Mouse::mouse = NULL;

Mouse *Mouse::getMouse()
{
	if (mouse == NULL)
		mouse = new Mouse();

	return mouse;
}

void Mouse::cleanUp()
{
	if (mouse != NULL)
	{
		delete mouse;
		mouse = NULL;
	}
}

Mouse::Mouse() : lastX(0), lastY(0), diffX(0), diffY(0), buttons(new bool[3])
{
	buttons[0] = buttons[1] = buttons[2] = false;
}

void Mouse::setLastMousePos(int x, int y)
{
	diffX = x - lastX;
	diffY = y - lastY;
	lastX = x;
	lastY = y;
}



////////////////////////////////////////////////////////////////////////////////
// -------------------- Global Input-Handling Functions --------------------- //
////////////////////////////////////////////////////////////////////////////////

// Reads any key that emits a character.
void updateKeyboard(unsigned char key, int x, int y)
{
	if ('a' <= key && key <= 'z')
		Keyboard::getKeyboard()->setDown(key - 'a');
	else if ('A' <= key && key <= 'Z')
		Keyboard::getKeyboard()->setDown(key - 'A');
	else if ('0' <= key && key <= '9')
		Keyboard::getKeyboard()->setDown(key - '0');
	else if (key == ' ')
		Keyboard::getKeyboard()->setDown(SR_KEY_SPACE);
	else if (key == 13)
		Keyboard::getKeyboard()->setDown(SR_KEY_ENTER);
	else
		std::cout << "got a \"normal\" " << (int)key << " down." << std::endl;
}

void updateKeyboardUp(unsigned char key, int x, int y)
{
	if ('a' <= key && key <= 'z')
		Keyboard::getKeyboard()->setUp(key - 'a');
	else if ('A' <= key && key <= 'Z')
		Keyboard::getKeyboard()->setUp(key - 'A');
	else if ('0' <= key && key <= '9')
		Keyboard::getKeyboard()->setUp(key - '0');
	else if (key == ' ')
		Keyboard::getKeyboard()->setUp(SR_KEY_SPACE);
	else if (key == 13)
		Keyboard::getKeyboard()->setUp(SR_KEY_ENTER);
	else
		std::cout << "got a \"normal\" " << (int)key << " up." << std::endl;
}

// Reads the arrow, F- and special keys.
void udpateSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1: Keyboard::getKeyboard()->setDown(SR_KEY_F1); break;
		case GLUT_KEY_F2: Keyboard::getKeyboard()->setDown(SR_KEY_F2); break;
		case GLUT_KEY_F3: Keyboard::getKeyboard()->setDown(SR_KEY_F3); break;
		case GLUT_KEY_F4: Keyboard::getKeyboard()->setDown(SR_KEY_F4); break;
		case GLUT_KEY_F5: Keyboard::getKeyboard()->setDown(SR_KEY_F5); break;
		case GLUT_KEY_F6: Keyboard::getKeyboard()->setDown(SR_KEY_F6); break;
		case GLUT_KEY_F7: Keyboard::getKeyboard()->setDown(SR_KEY_F7); break;
		case GLUT_KEY_F8: Keyboard::getKeyboard()->setDown(SR_KEY_F8); break;
		case GLUT_KEY_F9: Keyboard::getKeyboard()->setDown(SR_KEY_F9); break;
		case GLUT_KEY_F10: Keyboard::getKeyboard()->setDown(SR_KEY_F10); break;
		case GLUT_KEY_F11: Keyboard::getKeyboard()->setDown(SR_KEY_F11); break;
		case GLUT_KEY_F12: Keyboard::getKeyboard()->setDown(SR_KEY_F12); break;

		case GLUT_KEY_LEFT: Keyboard::getKeyboard()->setDown(SR_KEY_LEFT); break;
		case GLUT_KEY_UP: Keyboard::getKeyboard()->setDown(SR_KEY_UP); break;
		case GLUT_KEY_RIGHT: Keyboard::getKeyboard()->setDown(SR_KEY_RIGHT); break;
		case GLUT_KEY_DOWN: Keyboard::getKeyboard()->setDown(SR_KEY_DOWN); break;

		case GLUT_KEY_PAGE_UP: Keyboard::getKeyboard()->setDown(SR_KEY_PAGE_UP); break;
		case GLUT_KEY_PAGE_DOWN: Keyboard::getKeyboard()->setDown(SR_KEY_PAGE_DOWN); break;
		case GLUT_KEY_HOME: Keyboard::getKeyboard()->setDown(SR_KEY_HOME); break;
		case GLUT_KEY_END: Keyboard::getKeyboard()->setDown(SR_KEY_END); break;
		case GLUT_KEY_INSERT: Keyboard::getKeyboard()->setDown(SR_KEY_INSERT); break;
	}
}

void updateSpecialKeysUp(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1: Keyboard::getKeyboard()->setUp(SR_KEY_F1); break;
		case GLUT_KEY_F2: Keyboard::getKeyboard()->setUp(SR_KEY_F2); break;
		case GLUT_KEY_F3: Keyboard::getKeyboard()->setUp(SR_KEY_F3); break;
		case GLUT_KEY_F4: Keyboard::getKeyboard()->setUp(SR_KEY_F4); break;
		case GLUT_KEY_F5: Keyboard::getKeyboard()->setUp(SR_KEY_F5); break;
		case GLUT_KEY_F6: Keyboard::getKeyboard()->setUp(SR_KEY_F6); break;
		case GLUT_KEY_F7: Keyboard::getKeyboard()->setUp(SR_KEY_F7); break;
		case GLUT_KEY_F8: Keyboard::getKeyboard()->setUp(SR_KEY_F8); break;
		case GLUT_KEY_F9: Keyboard::getKeyboard()->setUp(SR_KEY_F9); break;
		case GLUT_KEY_F10: Keyboard::getKeyboard()->setUp(SR_KEY_F10); break;
		case GLUT_KEY_F11: Keyboard::getKeyboard()->setUp(SR_KEY_F11); break;
		case GLUT_KEY_F12: Keyboard::getKeyboard()->setUp(SR_KEY_F12); break;

		case GLUT_KEY_LEFT: Keyboard::getKeyboard()->setUp(SR_KEY_LEFT); break;
		case GLUT_KEY_UP: Keyboard::getKeyboard()->setUp(SR_KEY_UP); break;
		case GLUT_KEY_RIGHT: Keyboard::getKeyboard()->setUp(SR_KEY_RIGHT); break;
		case GLUT_KEY_DOWN: Keyboard::getKeyboard()->setUp(SR_KEY_DOWN); break;

		case GLUT_KEY_PAGE_UP: Keyboard::getKeyboard()->setUp(SR_KEY_PAGE_UP); break;
		case GLUT_KEY_PAGE_DOWN: Keyboard::getKeyboard()->setUp(SR_KEY_PAGE_DOWN); break;
		case GLUT_KEY_HOME: Keyboard::getKeyboard()->setUp(SR_KEY_HOME); break;
		case GLUT_KEY_END: Keyboard::getKeyboard()->setUp(SR_KEY_END); break;
		case GLUT_KEY_INSERT: Keyboard::getKeyboard()->setUp(SR_KEY_INSERT); break;
	}
}

// Start Screen-specific one-hit actions. ---------------------
void startScreenKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 'Q':
		case 27:  // Esc
			cleanup();
			break;

		case 's':
		case 'S':
			initNewGame();
			break;
	}
}

// Tactical-specific one-hit actions. -------------------------
void tacticalKeyboard(unsigned char key, int x, int y)
{
	Ship* enemy1;
	switch (key)
	{
		case 'k':
		case 'K':
			if (!enemyShips.empty())
				enemyShips.at(0)->destroy();
			break;
		case '{':
		case '[':
			enemy1 = new BasicBlueShip(new Blaster(), new BasicHull(), new BasicShield());
			enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
			env->add(enemy1);
			enemyShips.push_back(enemy1);
			break;
		case '}':
		case ']':
			enemy1 = new BasicRedShip(new Blaster(), new BasicHull(), new BasicShield());
			enemy1->setAt(rr(-125, 125), rr(-125, 125), rr(-125, 125));
			env->add(enemy1);
			enemyShips.push_back(enemy1);
			break;
//		case 't':
//		case 'T':
		case 13: // Enter
			if (playerShip->ammoPcnt() > 0)
				playerShip->fire();
			break;
//		case 'q':
//		case 'Q':
		case 27:  // Esc
			cleanup();
			break;
		case 'p':
		case 'P':
			paused = !paused;
			break;
		case 'v':
		case 'V':
			Camera::getCamera()->toggleDefault();
			break;

		case 'z':
		case 'Z':
			zoom = ((zoom + 1) % 3);
			break;

		case 'x':
		case 'X':
			playerShip->toggleTractorBeam();
			break;
	}
}

void tacticalKeyboardUp(unsigned char key, int x, int y) {}

void tacticalSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F8:
			cout << "Resetting." << endl;
			initTactical();
			break;

		case GLUT_KEY_F9:
			cout << "player's pos: " << playerShip->getPos().str() << endl;
			break;

		case GLUT_KEY_F10:
			cout << "Health: " << (100 * playerShip->hlthPcnt()) << (char)0x25 << endl;
			break;

		case GLUT_KEY_F11:
			cout << "Kill the player!" << endl;
			playerShip->hurt(999999);
			break;

		case GLUT_KEY_F12:
			cout << "Min bound of Leaf: " << playerShip->getResidence()->minBound.str() << endl
			     << "Max bound of Leaf: " << playerShip->getResidence()->maxBound.str() << endl;
			break;

		case GLUT_KEY_HOME:
			playerShip->getResidence()->remove(playerShip);
			playerShip->setAt(0,0,0);
			env->add(playerShip);
			break;
	}
}

void tacticalSpecialKeysUp(int key, int x, int y) {}

// Game Over-specific one-hit actions. ------------------------
void gameOverKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 'Q':
		case 27:  // Esc
			cleanup();
			break;

		case 's':
		case 'S':
			initNewGame();
			break;
	}
}

// Jam:
// I use the keyboard here for the purpose of getting multiple
// key presses and constant feedback, as opposed to the callback
// mechanism of the glutXXXFunc()'s.
void handleTacticalInput()
{
	if (playerShip->fuelPcnt() > 0)
	{
		// Yaw.
		if (Keyboard::getKeyboard()->isDown(SR_YAW_LEFT_1) ||
			Keyboard::getKeyboard()->isDown(SR_YAW_LEFT_2) ||
			Keyboard::getKeyboard()->isDown(SR_YAW_LEFT_3))
			playerShip->yawLeft();
		if (Keyboard::getKeyboard()->isDown(SR_YAW_RIGHT_1) ||
			Keyboard::getKeyboard()->isDown(SR_YAW_RIGHT_2) ||
			Keyboard::getKeyboard()->isDown(SR_YAW_RIGHT_3))
			playerShip->yawRight();

		// Acceleration.
		if (Keyboard::getKeyboard()->isDown(SR_ACCELERATE_1) ||
			Keyboard::getKeyboard()->isDown(SR_ACCELERATE_2) ||
			Keyboard::getKeyboard()->isDown(SR_ACCELERATE_3))
			playerShip->accelerate();
		if (Keyboard::getKeyboard()->isDown(SR_DECELERATE_1) ||
			Keyboard::getKeyboard()->isDown(SR_DECELERATE_2) ||
			Keyboard::getKeyboard()->isDown(SR_DECELERATE_3))
			playerShip->decelerate();
		if (Keyboard::getKeyboard()->isDown(SR_BREAK_1) ||
			Keyboard::getKeyboard()->isDown(SR_BREAK_2) ||
			Keyboard::getKeyboard()->isDown(SR_BREAK_3))
			playerShip->stabilize();

		// Pitch.
		if (Keyboard::getKeyboard()->isDown(SR_PITCH_BACK_1) ||
			Keyboard::getKeyboard()->isDown(SR_PITCH_BACK_2) ||
			Keyboard::getKeyboard()->isDown(SR_PITCH_BACK_3))
			playerShip->pitchBack();
		if (Keyboard::getKeyboard()->isDown(SR_PITCH_FORWARD_1) ||
			Keyboard::getKeyboard()->isDown(SR_PITCH_FORWARD_2) ||
			Keyboard::getKeyboard()->isDown(SR_PITCH_FORWARD_3))
			playerShip->pitchForward();

		// Roll.
		if (Keyboard::getKeyboard()->isDown(SR_ROLL_LEFT_1) ||
			Keyboard::getKeyboard()->isDown(SR_ROLL_LEFT_2) ||
			Keyboard::getKeyboard()->isDown(SR_ROLL_LEFT_3))
			playerShip->rollLeft();
		if (Keyboard::getKeyboard()->isDown(SR_ROLL_RIGHT_1) ||
			Keyboard::getKeyboard()->isDown(SR_ROLL_RIGHT_2) ||
			Keyboard::getKeyboard()->isDown(SR_ROLL_RIGHT_3))
			playerShip->rollRight();
	}
}


////////////////////////////////////////////////////////////////////////////////
// -------------------------- Glut callbacks -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void readKeyboard(unsigned char key, int x, int y)
{
	switch (screenState)
	{
		case START_SCREEN:
			startScreenKeyboard(key, x, y);
			break;
		case TACTICAL:
			tacticalKeyboard(key, x, y);
			break;
		case GAME_OVER:
			gameOverKeyboard(key, x, y);
			break;
	}

	updateKeyboard(key, x, y);
}

void readKeyboardUp(unsigned char key, int x, int y)
{
	switch (screenState)
	{
		case TACTICAL:
			tacticalKeyboardUp(key, x, y);
			break;
	}

	updateKeyboardUp(key, x, y);
}

void readSpecialKeys(int key, int x, int y)
{
	switch (screenState)
	{
		case TACTICAL:
			tacticalSpecialKeys(key, x, y);
			break;
	}

	udpateSpecialKeys(key, x, y);
}

void readSpecialKeysUp(int key, int x, int y)
{
	switch (screenState)
	{
		case TACTICAL:
			tacticalSpecialKeysUp(key, x, y);
			break;
	}

	updateSpecialKeysUp(key, x, y);
}


void processHits(GLint hits, GLuint buffer[])
{
	menu->processHits(hits, buffer);
}

void mouseClick(int button, int state, int x, int y)
{
	#define BUFSIZE 512

	GLuint selectBuffer[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuffer);

	#undef BUFSIZE

	(void) glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);

	gluOrtho2D(0,screen_width , 0,screen_height);
	menu->draw(GL_SELECT);
	hits = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	if(hits == 1)
		processHits(hits, selectBuffer);
}

void mouseButtHandler(int button, int state, int x, int y)
{
	if (screenState == TACTICAL)
	{
		// Things to do when the button is pushed.
		if (state == GLUT_DOWN)
		{
			Mouse::getMouse()->setLastMousePos(x, y);
			Camera::getCamera()->setMode(CAMERA_MODE_LOOK);
		}
		// Things to do when the button is released.
		else // state == GLUT_UP
		{
			Camera::getCamera()->setMode(CAMERA_MODE_DEFAULT);
		}
	}
	else //if (screenState == START_SCREEN)
	{
		mouseClick(button,state,x,y);
	}
}

void mouseMoveHandler(int x, int y)
{
	switch (screenState)
	{
		case TACTICAL:
			Mouse::getMouse()->setLastMousePos(x, y);
			break;
	}
}

