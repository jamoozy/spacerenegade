#include <GL/glut.h>
#include <iostream>
#include "environment.h"
#include "globals.h"
#include "camera.h"
#include "input.h"
#include "ship.h"
#include "button.h"

extern int screenState;

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
extern OctTree *env;


/////////////////////////////////////
// Global Input-Handling Functions //
/////////////////////////////////////

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
	else
		std::cout << "got a " << key << " down." << std::endl;
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
	else
		std::cout << "got a " << key << " up." << std::endl;
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


// Tactical-specific one-hit actions.
void tacticalKeyboard(unsigned char key, int x, int y)
{
	if (key == 't' || key == 'T')
		playerShip->fire();
}

void tacticalKeyboardUp(unsigned char key, int x, int y)
{
}

void tacticalSpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_F9)
		std::cout << playerShip->getPos().str() << std::endl;

	if (key == GLUT_KEY_F12)
		std::cout << "Min bound of Leaf: " << playerShip->getResidence()->min.str() << std::endl
		          << "Max bound of Leaf: " << playerShip->getResidence()->max.str() << std::endl;

	if (key == GLUT_KEY_HOME)
	{
		playerShip->getResidence()->remove(playerShip);
		playerShip->setAt(0,0,0);
		env->add(playerShip);
	}
}

void tacticalSpecialKeysUp(int key, int x, int y)
{
}


////////////////////////////////////////////////////////////////////////////////
// -------------------------- Glut callbacks -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void readKeyboard(unsigned char key, int x, int y)
{
	if (screenState == TACTICAL)
		tacticalKeyboard(key, x, y);

	updateKeyboard(key, x, y);
}

void readKeyboardUp(unsigned char key, int x, int y)
{
	if (screenState == TACTICAL)
		tacticalKeyboardUp(key, x, y);

	updateKeyboardUp(key, x, y);
}

void readSpecialKeys(int key, int x, int y)
{
	if (screenState == TACTICAL)
		tacticalSpecialKeys(key, x, y);

	udpateSpecialKeys(key, x, y);
}

void readSpecialKeysUp(int key, int x, int y)
{
	if (screenState == TACTICAL)
		tacticalSpecialKeysUp(key, x, y);

	updateSpecialKeysUp(key, x, y);
}


extern Button *buttons;
extern int numButtons;

void processHits(GLint hits, GLuint buffer[])
{
	for(int i = 0; i < hits; i++)
		for (int j = 0; j < numButtons; j++)
			if (buffer[i*4 + 3] == buttons[j].getID())
				buttons[j].buttonPressed();
}

#define BUFSIZE 512////////////////
//extern struct perspectiveData pD;

void mouseClick(int button, int state, int x, int y)
{
	GLuint selectBuffer[BUFSIZE];
	GLint hits;
	GLint viewport[4];
	
	if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glSelectBuffer(BUFSIZE, selectBuffer);
	(void) glRenderMode(GL_SELECT);
	
	glInitNames();
	glPushName(0);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluPickMatrix((GLdouble)x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport);

	int width = 1024;
	int height = 768;
	float aspect = (float)width/(float)height;

	glFrustum(-0.1 * aspect, 0.1 * aspect, -0.1, 0.1, 0.1, 20.0);
	drawButtons(GL_SELECT);
	//hits = glRenderMode(GL_RENDER);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	
	hits = glRenderMode(GL_RENDER);
	printf("Names = %d\nArea =  %d\n", selectBuffer[0], selectBuffer[3]);
	if(hits == 1) processHits(hits, selectBuffer);
	//printf("hits = %d\n", hits);
}

void mouseButtHandler(int button, int state, int x, int y)
{
	if (screenState == TACTICAL)
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
	else if (screenState == START_SCREEN)
	{
		mouseClick(button,state,x,y);
	}
}

void mouseMoveHandler(int x, int y)
{
	Mouse::getMouse()->setLastMousePos(x, y);
}

