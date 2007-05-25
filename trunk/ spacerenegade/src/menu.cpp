#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "object.h"
#include "button.h"
#include "menu.h"
//#include "main.cpp"

Menu::Menu(string type) :
	type(type)
	{
		if (type == "startScreen")
		{
			numButtons = 4;
			buttons = new Button[numButtons];
			//buttons[0] = Button("New Game", 0.1 , -10,50   , 0.6,0.8,0.2 , 1, initTactical); //error C2065: 'initTactical' : undeclared identifier
			buttons[1] = Button("Load Game",0.1 , -10,0    , 0.6,0.8,0.2 , 2,NULL);
			buttons[2] = Button("Options",  0.1 , -10,-50  , 0.6,0.8,0.2 , 3,NULL);
			//buttons[3] = Button("Quit",     0.1 , -10,-100 , 0.6,0.8,0.2 , 4,cleanup); //error C2065: 'cleanup' : undeclared identifier
		}
		else // other screens...
		{
			buttons = new Button[0];
		}
	}

Menu::~Menu() {}

void Menu::draw(GLenum mode)
{
	for (int i = 0; i < numButtons; i++)
		buttons[i].Place(mode);
}