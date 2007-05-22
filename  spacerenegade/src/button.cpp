#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include "button.h"

using std::string;

extern int screen_width;
extern int screen_height;

static GLfloat FONTCOEF = 0.013033; // The ratio between glRectf(..) and DrawText(..) coordinates
static GLfloat FONTWIDTH = 9; // We're using GLUT_BITMAP_9_BY_15
static GLfloat FONTHEIGHT = 15; // We're using GLUT_BITMAP_9_BY_15

Button::Button() :
	buffer(0),
	xPos(0),
	yPos(0),
	id(-1),
	pressFunc(NULL)
{
	RGB[0] = 0;
	RGB[1] = 0;
	RGB[2] = 0;
}
Button::Button(string title, GLfloat buffer, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue, GLuint id, void (*pressFunc)()) :
	title(title), buffer(buffer), xPos(xPos), yPos(yPos), id(id), pressFunc(pressFunc)
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
		glLoadName(id);
		glRectf((xPos * FONTCOEF) - buffer, 
				(yPos * FONTCOEF) - buffer, 
				(xPos * FONTCOEF) + buffer + (numOfLetters * FONTWIDTH * FONTCOEF), 
				(yPos * FONTCOEF) + buffer + (FONTHEIGHT * FONTCOEF));
	}
	else if (mode == GL_RENDER)
	{
		glRectf((xPos * FONTCOEF) - buffer, 
				(yPos * FONTCOEF) - buffer, 
				(xPos * FONTCOEF) + buffer + (numOfLetters * FONTWIDTH * FONTCOEF), 
				(yPos * FONTCOEF) + buffer + (FONTHEIGHT * FONTCOEF));
		DrawText((int)floor(xPos), (int)floor(yPos), title, 1.0, 1.0, 1.0);
	}
}

void Button::DrawText(GLint x, GLint y, string s, GLfloat r, GLfloat g, GLfloat b)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
		0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r,g,b);
	x = (int)floor(x + (.5 * screen_width));
	y = (int)floor(y + (.5 * screen_height));
	glRasterPos2i(x, y);
	for(unsigned int i = 0, lines = 0; i < s.size(); i++) {
		if (s.at(i) == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*15));
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s.at(i));
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}