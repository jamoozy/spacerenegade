#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
using std::string;

class Button
{
private:
	string title;
	GLfloat border; // how much space around button (.1 looks pretty good)
	GLfloat xPos;
	GLfloat yPos;
	GLfloat RGB[3];
	GLuint id;         // The ID number (for picking purposes).
	void (*pressFunc)(); // Function to call when button is pressed.

public:
	Button();
	Button(string title, GLfloat border,
	       GLfloat xPos, GLfloat yPos,
	       GLfloat red, GLfloat green, GLfloat blue,
	       GLuint id, void (*pressFunc)());
	virtual ~Button();

	void Place(GLenum mode);
	void drawText(GLint x, GLint y, string s, GLfloat r, GLfloat g, GLfloat b);

	GLfloat getBuffer(){return border;}
	string getTitle(){return title;}
	GLfloat getXPos(){return xPos;}
	GLfloat getYPos(){return yPos;}
	GLuint getID() { return id; }

	void buttonPressed();
	void setBuffer(GLfloat border){border = border;}
	void setTitle(string title){title = title;}
	void setXPos(GLfloat xPos){xPos = xPos;}
	void setYPos(GLfloat yPos){yPos = yPos;}
	void setRGB(GLfloat red, GLfloat green,	GLfloat blue){RGB[0] = red;RGB[1] = green;RGB[2] = blue;}
};

class Menu
{
private:
	Button *buttons;
	int numButtons;
	int type;

public:
	Menu();
	Menu(int type);
	virtual ~Menu();
	void setProjection();
	void draw(GLenum mode);
	void cleanProjection();
	//int getNumButtons() {return numButtons;}
	void processHits(GLint hits, GLuint buffer[]);
};

#endif

