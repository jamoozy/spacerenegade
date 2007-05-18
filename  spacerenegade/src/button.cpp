#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "button.h"
using std::string;

Button::Button()
:length(0), width(0), xPos(0), yPos(0)
{
	RGB = new GLfloat[3];
	RGB[0] = 0;
	RGB[1] = 0;
	RGB[2] = 0;
}
Button::Button(char title[], GLfloat length, GLfloat width, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue)
:title(title), length(length), width(width), xPos(xPos), yPos(yPos)
{
	RGB = new GLfloat[3];
	RGB[0] = red;
	RGB[1] = green;
	RGB[2] = blue;
}

void Button::Place()
{
	glColor3f(RGB[0], RGB[1], RGB[2]);
    glRectf( xPos, yPos, xPos + width, yPos + length);
	//DrawText(2, -4, "ASL;DFJSIL;AGJ",1.0, 0.0, 0.0);
	//"test title", 2, 2, -4, 2, .6, .8, .2
	DrawText(xPos + (.5 * width),
			 yPos + (.5 * length),
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
	glRasterPos2i(x, y);
	for(p = s, lines = 0; *p; p++) {
		if (*p == '\n') {
			lines++;
			glRasterPos2i(x, y-(lines*18));
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}