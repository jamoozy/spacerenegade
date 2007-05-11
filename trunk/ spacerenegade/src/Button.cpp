#include <GL/glut.h>
#include <stdio.h>
#include "Button.h"
using std::string;

Button::Button()
:width(0), length(0), xPos(0), yPos(0)
{
	RGB = new GLfloat[3];
	RGB[0] = 0;
	RGB[1] = 0;
	RGB[2] = 0;
}
Button::Button(string title, GLfloat length, GLfloat width, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue)
:width(width), length(length), xPos(xPos), yPos(yPos),title(title)
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
}
