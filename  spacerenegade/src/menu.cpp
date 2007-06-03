#include "GL/glut.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include "display.h"
#include "menu.h"
#include "mission.h"

using std::string;
using std::cout;
using std::endl;

extern int screen_width;
extern int screen_height;
extern GLfloat miniMapX;
extern GLfloat miniMapY;

extern Menu *menu;

static GLfloat FONTWIDTH = 9; // We're using GLUT_BITMAP_9_BY_15
static GLfloat FONTHEIGHT = 15; // We're using GLUT_BITMAP_9_BY_15

void cleanup();

////////////////////////////////////////////////////////////////////////////////
// --------------------------- Button Definition ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

Button::Button() : border(0), xPos(0), yPos(0), id(-1), pressFunc(NULL)
{
	RGB[0] = 0;
	RGB[1] = 0;
	RGB[2] = 0;
}

Button::Button(string title, GLfloat border, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue, GLuint id, void (*pressFunc)()) :
	title(title), border(border), xPos(xPos), yPos(yPos), id(id), pressFunc(pressFunc)
{
	RGB[0] = red;
	RGB[1] = green;
	RGB[2] = blue;
}

Button::~Button()
{
}

void Button::buttonPressed()
{
	if (pressFunc == NULL)
		std::cout << title << " button not yet implemented!" << std::endl;
	else
		(*pressFunc)();
}

void Button::drawText(GLint x, GLint y, string s, GLfloat r, GLfloat g, GLfloat b)
{
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r,g,b);
	glRasterPos2i(x, y);
	for(unsigned int i = 0, lines = 0; i < s.size(); i++) {
		if (s.at(i) == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*15));
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s.at(i));
	}
	glPopMatrix();
}

void Button::Place(GLenum mode)
{
	GLfloat numOfLetters = title.length();
	glColor3f(RGB[0], RGB[1], RGB[2]);
	if (mode == GL_SELECT)
	{
		glLoadName(id);
		glRectf((xPos) - border, 
				(yPos) - border, 
				(xPos) + border + (numOfLetters * FONTWIDTH), 
				(yPos) + border + (FONTHEIGHT));
	}
	else if (mode == GL_RENDER)
	{
		glRectf((xPos) - border,
				(yPos) - border,
				(xPos) + border + (numOfLetters * FONTWIDTH), 
				(yPos) + border + (FONTHEIGHT));
		drawText((int)floor(xPos), (int)floor(yPos), title, 1.0, 1.0, 1.0);
	}
}



////////////////////////////////////////////////////////////////////////////////
// --------------------------- Menu Definition ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(int type) : type(type)
{
	int height, NUM_MISSIONS; // replace with something more dynamic
	switch (type)
	{
		case START_SCREEN:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.6,0.8,0.2 , 1, initTactical);
			buttons[1] = Button("Load Game",5 , 500,400 , 0.6,0.8,0.2 , 2, NULL);
			buttons[2] = Button("Options",  5 , 500,350 , 0.6,0.8,0.2 , 3, NULL);
			buttons[3] = Button("Quit",     5 , 500,300 , 0.6,0.8,0.2 , 4, cleanup);
			break;

		case TACTICAL:
			numButtons = 2;
			buttons = new Button[numButtons];
			buttons[0] = Button("-", .1, miniMapX - 85, miniMapY - 90, 0, 0, .2, 1, NULL);
			buttons[1] = Button("+", .1, miniMapX + 85, miniMapY - 90, 0, 0, .2, 2, NULL);
			break;

		case MISSION_BOARD: // (Gum)
			NUM_MISSIONS = 2; // FIXME replace with something more dynamic
			height = 50;
			numButtons = 2 + NUM_MISSIONS;

			//draw the two main buttons
			//buttons [0] = Button("Accept Mission", 5, 200, 500, .4,.5,.7, 1, acceptMission);
			buttons [1] = Button("Exit", 5, 400, 500, .4,.5,.7, 1, initPlanet);

			//draw mission titles
			for (int i = 0; i < NUM_MISSIONS; i ++)
			{
				//Mission m = listOfAvailableMissions.get(i); // fix, obviously
				//buttons[i + 2] = Button(m.getTitle(), 1, 100, height, .1,.1,.1, i+2, displayMissionBriefing);
				height += 50;
			}
			// handle displaying briefing, objectives, and reward when mission title is clicked
			break;

		case GAME_OVER:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.8,0.2,0.1 , 1, initTactical);
			buttons[1] = Button("Load Game",5 , 500,400 , 0.8,0.2,0.1 , 2, NULL);
			buttons[2] = Button("Options",  5 , 500,350 , 0.8,0.2,0.1 , 3, NULL);
			buttons[3] = Button("Quit",     5 , 500,300 , 0.8,0.2,0.1 , 4, cleanup);
			break;
	}
}

Menu::~Menu()
{
	delete [] buttons;
}

void Menu::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, screen_width, 0.0, screen_height);
	glMatrixMode(GL_MODELVIEW);
}

void Menu::draw(GLenum mode)
{
	if (mode != GL_SELECT)
		setProjection();

	for (int i = 0; i < numButtons; i++)
		buttons[i].Place(mode);

	if (mode != GL_SELECT)
		cleanProjection();
}

void Menu::cleanProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Menu::processHits(GLint hits, GLuint buffer[])
{
	for(int i = 0; i < hits; i++)
		for (int j = 0; j < numButtons; j++)
			if (buffer[i*4 + 3] == buttons[j].getID())
				buttons[j].buttonPressed();
}
