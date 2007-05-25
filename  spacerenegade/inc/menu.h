#ifndef MENU_H
#define MENU_H

//#include "Model_3DS.h" //
#include "object.h" //
#include "vec3.h" //
#include "button.h" //

#include <iostream>
#include <string>
using namespace std;

class Menu : public Object
{
private:
	Button *buttons;
	int numButtons;
	string type;

public:
	Menu();
	Menu(string type);
	virtual ~Menu();
	void draw(GLenum mode);

};

#endif