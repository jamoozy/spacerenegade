#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "button.h"

extern int screen_width;
extern int screen_height;

static GLfloat FONTCOEF = 0.013033; // The ratio between glRectf(..) and DrawText(..) coordinates
static GLfloat FONTWIDTH = 9; // We're using GLUT_BITMAP_9_BY_15
static GLfloat FONTHEIGHT = 15; // We're using GLUT_BITMAP_9_BY_15

using std::string;

Button::Button()
:buffer(0), xPos(0), yPos(0)
{
	RGB = new GLfloat[3];
	RGB[0] = 0;
	RGB[1] = 0;
	RGB[2] = 0;
}
Button::Button(char title[], GLfloat buffer, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue)
:title(title), buffer(buffer), xPos(xPos), yPos(yPos)
{
	RGB = new GLfloat[3];
	RGB[0] = red;
	RGB[1] = green;
	RGB[2] = blue;
}

void Button::Place()
{
	string s = title;
	GLfloat numOfLetters = s.length();
	glColor3f(RGB[0], RGB[1], RGB[2]);
    glRectf((xPos * FONTCOEF) - buffer, 
			(yPos * FONTCOEF) - buffer, 
			(xPos * FONTCOEF) + buffer + (numOfLetters * FONTWIDTH * FONTCOEF), 
			(yPos * FONTCOEF) + buffer + (FONTHEIGHT * FONTCOEF));
	DrawText(xPos,
			 yPos,
			 title,
			 1.0, 1.0, 1.0);
}

void Button::DrawText(GLint x, GLint y, char *s, GLfloat r, GLfloat g, GLfloat b)
{
	int lines;
	char* p;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
		0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r,g,b);
	x = x + (.5 * screen_width);
	y = y + (.5 * screen_height);
	glRasterPos2i(x, y);
	for(p = s, lines = 0; *p; p++) {
		if (*p == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*15));
		}
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}