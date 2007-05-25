#ifndef MENU_H
#define MENU_H

//#include "Model_3DS.h" //
#include "object.h" //
#include "vec3.h" //
#include "button.h" //

#include <iostream>
#include <string>
using namespace std;

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
	void draw(GLenum mode);
	//int getNumButtons() {return numButtons;}
	void processHits(GLint hits, GLuint buffer[]);

};

#endif