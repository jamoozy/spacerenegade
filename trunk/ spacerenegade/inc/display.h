
#define DEBUG_MODE 1
#define PRINT_FPS 0
#define LIMIT_FPS 1

#ifndef DISPLAY_H
#define DISPLAY_H

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768

#include <AL/alut.h>
#include <string>
#include <vector>

using std::vector;
using std::string;

enum { START_SCREEN, OPTIONS_SCREEN, TACTICAL, PLANET, MISSION_BOARD, GAME_OVER };

class Sound
{
private:
	string name;
	ALuint buffer, source;

public:
	Sound() {};
	Sound(string name);

	void play();
	void stop();
	void setLooping();
	string getName() const {return name;};

	bool operator==(const string& name);

	void shiftMusic(string name);

};


class SoundFactory
{
private:
	vector<Sound> sounds;
	
public:
	SoundFactory() {};
	SoundFactory(string *names);

	void play(const string& name);
};

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
void drawText(GLint x, GLint y, std::string s, Color c, bool color);
void glCircle(GLfloat cx, GLfloat cy, GLfloat r, int side);
void resize(int w, int h);
void drawBox(int x1, int y1, int x2, int y2, double r, double g, double b); // (Gum)

// Display functions.
void display();
void displayStartScreen();
void displayOptionsScreen();
void displayTacticalPaused();
void displayTactical();
void adjustGlobalLighting();
void drawHUD();
void drawObjectives(); // (Gum)
void drawMeters();
void drawMiniMap();
void displayGameOver();
void displayMissionBoard();
void displayPlanet();
void pauseMenu(); // (Gum)

// Initializations.
void initStartScreen();
void initOptionsScreen();
void initNewGame();
void initTactical();
void initGameOver();
void initPlanet();
void initMissionBoard();

// Player state changers.
void refuelPlayer();
void reloadPlayerAmmo();
void healPlayer();
void sellMaterials();


#endif

