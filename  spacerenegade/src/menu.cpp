#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "object.h"
#include "button.h"
#include "menu.h"
#include "display.h"

void initTactical();
void initStartScreen();
void cleanup();

Menu::Menu(int type) :
	type(type)
	{
		switch (type)
		{
			case START_SCREEN:
				numButtons = 4;
				buttons = new Button[numButtons];
				buttons[0] = Button("New Game", 0.1 , -10,50   , 0.6,0.8,0.2 , 1, initTactical);
				buttons[1] = Button("Load Game",0.1 , -10,0    , 0.6,0.8,0.2 , 2,NULL);
				buttons[2] = Button("Options",  0.1 , -10,-50  , 0.6,0.8,0.2 , 3,NULL);
				buttons[3] = Button("Quit",     0.1 , -10,-100 , 0.6,0.8,0.2 , 4, cleanup);
				break;
			case MISSION:
				break;
			default: break;
		}
	}

Menu::~Menu() {}

void Menu::draw(GLenum mode)
{
	for (int i = 0; i < numButtons; i++)
		buttons[i].Place(mode);
}

void Menu::processHits(GLint hits, GLuint buffer[])
{
	for(int i = 0; i < hits; i++)
		for (int j = 0; j < numButtons; j++)
			if (buffer[i*4 + 3] == buttons[j].getID())
				buttons[j].buttonPressed();
}
