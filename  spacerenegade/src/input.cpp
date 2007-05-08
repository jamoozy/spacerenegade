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
	else if (key == ' ')
		Keyboard::getKeyboard()->setDown(SR_KEY_SPACE);
	else
		std::cout << "got a " << key << " for some reason." << std::endl;
}


void readKeyboardUp(unsigned char key, int x, int y)
{
	if ('a' <= key && key <= 'z')
		Keyboard::getKeyboard()->setUp(key - 'a');
	else if ('A' <= key && key <= 'Z')
		Keyboard::getKeyboard()->setUp(key - 'A');
	else if (key == ' ')
		Keyboard::getKeyboard()->setUp(SR_KEY_SPACE);
	else
		std::cout << "got a " << key << " for some reason." << std::endl;
}

void readSpecialKeys(int key, int x, int y)
{
//	glutPostRedisplay();
}

void mouseButtHandler(int button, int state, int x, int y)
{
//	glutPostRedisplay();
}

void mouseMoveHandler(int x, int y)
{
//	glutPostRedisplay();
}

