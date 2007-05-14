#include <GL/glut.h>
#include <iostream>
#include "environment.h"
#include "camera.h"
#include "input.h"
#include "ship.h"

Keyboard *Keyboard::keyboard = NULL;

Keyboard::Keyboard() : keys(new bool[SR_NUM_KEYS])
{
	for (int i = 0; i < SR_NUM_KEYS; i++)
		keys[i] = false;
}

Keyboard::~Keyboard()
{
	delete [] keys;
}

Keyboard *Keyboard::getKeyboard()
{
	if (keyboard == NULL)
		keyboard = new Keyboard();

	return keyboard;
}

bool Keyboard::isDown(int key) const
{
	return keys[key];
}

void Keyboard::setDown(int key)
{
	keys[key] = true;
}

void Keyboard::setUp(int key)
{
	keys[key] = false;
}

void Keyboard::cleanUp()
{
	delete keyboard;
}

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

Mouse::~Mouse()
{
	delete [] buttons;
}

void Mouse::setLastMousePos(int x, int y)
{
	diffX = x - lastX;
	diffY = y - lastY;
	lastX = x;
	lastY = y;
}

void Mouse::setDown(int button)
{
	buttons[button] = true;
}

void Mouse::setUp(int button)
{
	buttons[button] = false;
}

double Mouse::getDiffX() const
{
	return diffX;
}

double Mouse::getDiffY() const
{
	return diffY;
}

void Mouse::clearDiffs()
{
	diffX = diffY = 0;
}

extern Ship *playerShip;



/////////////////////////////////////
// Global Input-Handling Functions //
/////////////////////////////////////

// Reads any key that emits a character.
void readKeyboard(unsigned char key, int x, int y)
{
	if ('a' <= key && key <= 'z')
		Keyboard::getKeyboard()->setDown(key - 'a');
	else if ('A' <= key && key <= 'Z')
		Keyboard::getKeyboard()->setDown(key - 'A');
	else if ('0' <= key && key <= '9')
		Keyboard::getKeyboard()->setDown(key - '0');
	else if (key == ' ')
		Keyboard::getKeyboard()->setDown(SR_KEY_SPACE);
	else
		std::cout << "got a " << key << " down." << std::endl;
}


void readKeyboardUp(unsigned char key, int x, int y)
{
	if ('a' <= key && key <= 'z')
		Keyboard::getKeyboard()->setUp(key - 'a');
	else if ('A' <= key && key <= 'Z')
		Keyboard::getKeyboard()->setUp(key - 'A');
	else if ('0' <= key && key <= '9')
		Keyboard::getKeyboard()->setUp(key - '0');
	else if (key == ' ')
		Keyboard::getKeyboard()->setUp(SR_KEY_SPACE);
	else
		std::cout << "got a " << key << " up." << std::endl;
}

// Reads the arrow, F- and special keys.
void readSpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_F9)
		std::cout << playerShip->getPos().str() << std::endl;

	if (key == GLUT_KEY_F12)
		std::cout << "Min bound of Leaf: " << playerShip->getResidence()->min.str() << std::endl
		          << "Max bound of Leaf: " << playerShip->getResidence()->max.str() << std::endl;

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

		case SR_KEY_PAGE_UP: Keyboard::getKeyboard()->setDown(SR_KEY_PAGE_UP); break;
		case SR_KEY_PAGE_DOWN: Keyboard::getKeyboard()->setDown(SR_KEY_PAGE_DOWN); break;
		case SR_KEY_HOME: Keyboard::getKeyboard()->setDown(SR_KEY_HOME); break;
		case SR_KEY_END: Keyboard::getKeyboard()->setDown(SR_KEY_END); break;
		case SR_KEY_INSERT: Keyboard::getKeyboard()->setDown(SR_KEY_INSERT); break;
	}
}

void readSpecialKeysUp(int key, int x, int y)
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

		case SR_KEY_PAGE_UP: Keyboard::getKeyboard()->setUp(SR_KEY_PAGE_UP); break;
		case SR_KEY_PAGE_DOWN: Keyboard::getKeyboard()->setUp(SR_KEY_PAGE_DOWN); break;
		case SR_KEY_HOME: Keyboard::getKeyboard()->setUp(SR_KEY_HOME); break;
		case SR_KEY_END: Keyboard::getKeyboard()->setUp(SR_KEY_END); break;
		case SR_KEY_INSERT: Keyboard::getKeyboard()->setUp(SR_KEY_INSERT); break;
	}
}

// Right now all this does is go into look-around mode
// when any button is held down.
void mouseButtHandler(int button, int state, int x, int y)
{
	// Things to do when the button is pushed.
	if (state == GLUT_UP)
	{
		Camera::getCamera()->setMode(CAMERA_MODE_FOLLOW);
	}
	// Things to do when the button is released.
	else // state == GLUT_DOWN
	{
		Mouse::getMouse()->setLastMousePos(x, y);
		Camera::getCamera()->setMode(CAMERA_MODE_LOOK);
	}
}

void mouseMoveHandler(int x, int y)
{
	Mouse::getMouse()->setLastMousePos(x, y);
}
