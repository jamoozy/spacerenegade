#include "GL/glut.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include "display.h"
#include "menu.h"

using std::string;
using std::cout;
using std::endl;

extern int screen_width;
extern int screen_height;

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

void Button::Place(GLenum mode)
{
	GLfloat numOfLetters = title.length();
	glColor3f(RGB[0], RGB[1], RGB[2]);
	if (mode == GL_SELECT)
	{
//		cout << "there" << endl;

//		cout << "rect: (" << (xPos) - border << ", " 
//		     << (yPos) - border << "), -> ("
//		     << (xPos) + border + (numOfLetters * FONTWIDTH)
//		     << ", " << (yPos) + border + (FONTHEIGHT) << ')' << endl;

		glLoadName(id);
		glRectf((xPos) - border, 
				(yPos) - border, 
				(xPos) + border + (numOfLetters * FONTWIDTH), 
				(yPos) + border + (FONTHEIGHT));
//		menu->setProjection();
//		glRenderMode(GL_RENDER);
//		glRectf((xPos) - border, 
//				(yPos) - border, 
//				(xPos) + border + (numOfLetters * FONTWIDTH), 
//				(yPos) + border + (FONTHEIGHT));
//		glRenderMode(GL_SELECT);
//		menu->cleanProjection();
	}
	else if (mode == GL_RENDER)
	{
//		cout << "Modelview Matrix:" << endl;
//		GLdouble matrix[16];
//		glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
//		for (int i = 0; i < 4; i++) {
//			for (int j = 0; j < 4; j++) {
//				cout << j*4+i << ": " << matrix[j*4+i] << ",  ";
//			}
//			cout << endl;
//		}
//		cout << endl;
//		cout << "Projection Matrix:" << endl;
//		glGetDoublev(GL_PROJECTION_MATRIX, matrix);
//		for (int i = 0; i < 4; i++) {
//			for (int j = 0; j < 4; j++) {
//				cout << j*4+i << ": " << matrix[j*4+i] << ",  ";
//			}
//			cout << endl;
//		}
//		cout << endl;

		glRectf((xPos) - border,
				(yPos) - border,
				(xPos) + border + (numOfLetters * FONTWIDTH), 
				(yPos) + border + (FONTHEIGHT));
		DrawText((int)floor(xPos), (int)floor(yPos), title, 1.0, 1.0, 1.0);
	}
}

void Button::DrawText(GLint x, GLint y, string s, GLfloat r, GLfloat g, GLfloat b)
{
//	menu->setProjection();
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
//	menu->cleanProjection();
}



////////////////////////////////////////////////////////////////////////////////
// --------------------------- Menu Definition ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(int type) : type(type)
{
	switch (type)
	{
		case START_SCREEN:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.6,0.8,0.2 , 1, initTactical);
			buttons[1] = Button("Load Game",5 , 500,400 , 0.6,0.8,0.2 , 2,NULL);
			buttons[2] = Button("Options",  5 , 500,350 , 0.6,0.8,0.2 , 3,NULL);
			buttons[3] = Button("Quit",     5 , 500,300 , 0.6,0.8,0.2 , 4, cleanup);
			break;
		case MISSION:
			break;

		case GAME_OVER:
			numButtons = 4;
			buttons = new Button[numButtons];
			buttons[0] = Button("New Game", 5 , 500,450 , 0.8,0.2,0.1 , 1, initTactical);
			buttons[1] = Button("Load Game",5 , 500,400 , 0.8,0.2,0.1 , 2,NULL);
			buttons[2] = Button("Options",  5 , 500,350 , 0.8,0.2,0.1 , 3,NULL);
			buttons[3] = Button("Quit",     5 , 500,300 , 0.8,0.2,0.1 , 4, cleanup);
			break;
		default: break;
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
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
		0.0, glutGet(GLUT_WINDOW_HEIGHT), -50.1, 50.1);
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
