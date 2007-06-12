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

extern vector<Mission*> missionsAvailable;
extern vector<Mission*> missionsOn;

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
	for(unsigned int i = 0, lines = 0; i < s.size(); i++)
	{
		if (s.at(i) == '\n')
		{
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

Menu::Menu(int type) : type(type), selectedMission(NULL), remake(false)
{
	int height, NUM_MISSIONS; // replace with something more dynamic
	switch (type)
	{
		case START_SCREEN:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.6,0.8,0.2 , 1, initNewGame);
			buttons[1] = Button("Load Game",5 , 500,400 , 0.6,0.8,0.2 , 2, NULL);
			buttons[2] = Button("Options",  5 , 500,350 , 0.6,0.8,0.2 , 3, NULL);
			buttons[3] = Button("Quit",     5 , 500,300 , 0.6,0.8,0.2 , 4, cleanup);
			//buttons[4] = Button("TEST",     5 , 500,250 , 0.6,0.8,0.2 , 5, initPlanet);
			//buttons[5] = Button("Game Over",     5 , 500,200 , 0.6,0.8,0.2 , 6, initGameOver);
			break;

		case TACTICAL:
			//	Sound sound("music");
			//	sound.play();
			//numButtons = 2;
			//buttons = new Button[numButtons];
			//buttons[0] = Button("-", .1, miniMapX - 85, miniMapY, 0, 0, .2, 1, NULL);
			//buttons[1] = Button("+", .1, miniMapX + 85, miniMapY, 0, 0, .2, 2, NULL);
			break;

		case PLANET:
			numButtons = 5;
			buttons = new Button[numButtons];
			buttons[0] = Button("Mission Board", 5 , 500,450 , .3,.3,1 , 1, initMissionBoard);
			buttons[1] = Button("Refuel",5 , 500, 400 , .3,.3,1 , 2, refuelPlayer);
			buttons[2] = Button("More Ammo",5 , 500, 350 , .3,.3,1 , 3, reloadPlayerAmmo);
			buttons[3] = Button("Heal",5 , 500, 300 , .3,.3,1 , 4, healPlayer);
			buttons[4] = Button("Leave",5 , 500, 250 , .3,.3,1 , 5, initTactical);
			break;

		case MISSION_BOARD: // (Gum) (Note: this would be a lot easier if I could pass parameters into the functions I'm passing)
			NUM_MISSIONS = (int)missionsAvailable.size();
			height = 720;
			numButtons = 2 + NUM_MISSIONS;

			buttons = new Button[numButtons];
			//draw the two main buttons

			buttons [0] = Button("Accept Mission", 5, 150, 300, .1,.7,.1, -1, NULL); // Accept Mission will have id -1 (See processHits)
			buttons [1] = Button("Exit", 5, 500, 30, .4,.5,.7, 2, initPlanet);

			//draw mission titles
			for (int i = 0; i < NUM_MISSIONS; i ++)
			{
				Mission *m = missionsAvailable[i];
				buttons[i + 2] = Button(m->getTitle(), 1, 20, height, .1,.1,.1, (100 + i), NULL);
				height -= 30;
			}
			// handle displaying briefing, objectives, and reward when mission title is clicked
			break;

		case GAME_OVER:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.8,0.2,0.1 , 1, initNewGame);
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
	{
		if (selectedMission != NULL)
			selectedMission->displayMissionBriefing();

		cleanProjection();
	}
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
			{
				if (buttons[j].getID() == -1) // Accept Mission button was pressed
				{
					if (selectedMission == NULL)
					{
						cout << "1. No mission selected." << endl;
					}
					else
					{
						bool notYetOn = true;
						for (vector<Mission*>::iterator iter = missionsOn.begin();
							(iter != missionsOn.end()) &&
							(notYetOn = (*iter != selectedMission));
							iter++);

						// Only do this if we're not yet on.
						if (notYetOn)
						{
							missionsOn.push_back(selectedMission);
							soundFactory->play("missionaccepted");
							vector<Mission*>::iterator iter = missionsAvailable.begin();
							for (; selectedMission != *iter && iter != missionsAvailable.end(); iter++);
							missionsAvailable.erase(iter);
							remake = true;
						}

						// missionsAvailable.erase(selectedMission);
						// Remove selectedMission from available missions,
						// there's definitely a more efficient way to do this
						// than what I would hack out. (Gum)
					}
				}
				else if (buttons[j].getID() >= 100) // a displayMission button was pressed
				{
					selectedMission = missionsAvailable[buttons[j].getID() - 100];
				}
				else
					buttons[j].buttonPressed();
			}
}

