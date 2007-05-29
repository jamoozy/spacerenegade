
#ifndef DISPLAY_H
#define DISPLAY_H

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768

#include <string>

enum { START_SCREEN, TACTICAL, MISSION, GAME_OVER };

class Color
{
	double _r, _g, _b, _a;

public:
	Color();
	Color(double r, double g, double b);
	Color(double r, double g, double b, double a);

	double r() const { return _r; };
	double g() const { return _g; };
	double b() const { return _b; };
	double a() const { return _a; };
};

// Util-type functions.
double rr(double max, double min);
void drawText(GLint x, GLint y, std::string s, Color c);
void glCircle(GLfloat cx, GLfloat cy, GLfloat r, int side);
void resize(int w, int h);

// Display functions.
void display();
void displayStartScreen();
void displayTactical();
void adjustGlobalLighting();
void drawHUD();
void drawMiniMap();
void displayGameOver();

// Initializations.
void initStartScreen();
void initTactical();
void initGameOver();

#endif

