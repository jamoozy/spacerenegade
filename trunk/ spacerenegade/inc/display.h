
#ifndef DISPLAY_H
#define DISPLAY_H

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768

#include <string>

enum { START_SCREEN, TACTICAL, MISSION, GAME_OVER };

// Util-type functions.
double rr(double max, double min);
void drawText(GLint x, GLint y, std::string s, GLfloat r, GLfloat g, GLfloat b);
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

