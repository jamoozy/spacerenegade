#ifndef SHIP_H
#define SHIP_H

#include <string>
#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"
#include "weapon.h"

using std::string;

// General form of ships.  Rather abstract.
class Ship : public Object
{
protected:
	// This is an orthonormal rotation matrix that describes the
	// orientation of the ship.  With this matrix you can do
	// pitch, roll, and yaw all "correctly" (i.e. as you'd
	// expect them to look if you were really flying around in
	// a spaceship) and you can easily get the orientation of
	// the ship by selecting good values.
	Matrix lcs;  // Local Coordinate System.

	Weapon *weapon; // What this ship shoots with.
	double fuel;   // Amount of fuel left.

	Ship(char *modelName, Weapon *weapon, double fuel);

	GLdouble pitchF[16];
	GLdouble pitchB[16];
	GLdouble yawL[16];
	GLdouble yawR[16];
	GLdouble rollL[16];
	GLdouble rollR[16];

public:
	virtual ~Ship() { delete weapon; };

	// Rate functions.
	virtual double roa() const { return 0.005; }; // Rate of Acceleration
	virtual double rod() const { return 0.001; }; // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	virtual string getType() const { return "Ship"; };

	Vec3 getDir() const { return Vec3(lcs[8],lcs[9],lcs[10]); };
	Vec3 getUp() const { return Vec3(lcs[4],lcs[5],lcs[6]); };

	virtual void draw();

	// Movement.
	virtual void accelerate() { velocity += getDir() * roa(); };
	virtual void decelerate() { velocity -= getDir() * rod(); };
	virtual void stabilize();
	virtual void pitchBack();
	virtual void pitchForward();
	virtual void yawLeft();
	virtual void yawRight();
	virtual void rollLeft();
	virtual void rollRight();

	// Status stuff.
	virtual double maxHlth() const { return 1000; };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return weapon->maxAmmo(); };
	virtual double getHlth() const { return maxHlth() - damage; };
	virtual double getFuel() const { return fuel; };
	virtual double getAmmo() const { return weapon->getAmmo(); };
	virtual double hlthPcnt() const { return 1.0 - damage / maxHlth(); };
	virtual double fuelPcnt() const { return fuel / maxFuel(); };
	virtual double ammoPcnt() const { return weapon->getAmmo() / weapon->maxAmmo(); };
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------ Player's Ship ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Interface for the player's ship.  Very specific to the needs of the player.
class PShip : public Ship
{
	//bool lightRnotL;
	//int lightTime;
	GLTexture skymap;
	bool skymapLoaded;

public:
	PShip(Weapon *weapon);
	virtual ~PShip() {};
	virtual string getType() const { return "PShip"; };
	virtual void hits(Object *o);
	virtual void draw();
	virtual void fire() { weapon->fire(this); };

	// Ship methods that need some restrictions put on them.
	virtual void accelerate();
	virtual void decelerate();
	virtual void stabilize();
	virtual void yawLeft();
	virtual void yawRight();
	virtual void rollLeft();
	virtual void rollRight();
	virtual void pitchBack();
	virtual void pitchForward();

	// Rate functions inherited from Ship. They act as inheritable constants
	// for now, later they'll be based on engines and such.
	virtual double roa() const { return 0.005; };  // Rate of Acceleration
	virtual double rod() const { return 0.001; };  // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	// Status of the ship. Later these will be affected by the
	// capacity of the ship and such.
	virtual double maxHlth() const { return 1000; };
	virtual double maxFuel() const { return 10000; };
};

#endif

