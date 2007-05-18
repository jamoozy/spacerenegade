#ifndef BUTTON_H
#define BUTTON_H

//#include "vec3.h"
//#include <vector.h>
#include <string>
using std::string;

class Button
{
private:
	char* title;
	GLfloat length;
	GLfloat width;
	GLfloat xPos;
	GLfloat yPos;
	GLfloat* RGB;

public:
	Button();
	Button(char title[], GLfloat length, GLfloat width, GLfloat xPos, GLfloat yPos, GLfloat red, GLfloat green, GLfloat blue);

	void Place();
	void DrawText(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b);

	GLfloat getLength(){return length;}
	GLfloat getWidth(){return width;}
	string getTitle(){return title;}
	GLfloat getXPos(){return xPos;}
	GLfloat getYPos(){return yPos;}
	


	void setLength(GLfloat length){length = length;}
	void setWidth(GLfloat width){width = width;}
	void setTitle(string title){title = title;}
	void setXPos(GLfloat xPos){xPos = xPos;}
	void setYPos(GLfloat yPos){yPos = yPos;}
	void setRGB(GLfloat red, GLfloat green,	GLfloat blue){RGB[0] = red;RGB[1] = green;RGB[2] = blue;}

	

};

#endif
