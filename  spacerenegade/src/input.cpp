#include <GL/glut.h>
#include <iostream>
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

bool Keyboard::isDown(int key)
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

extern Ship *playerShip;

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

void readSpecialKeys(int key, int x, int y)
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

void mouseButtHandler(int button, int state, int x, int y)
{
//	glutPostRedisplay();
}

void mouseMoveHandler(int x, int y)
{
//	glutPostRedisplay();
}

