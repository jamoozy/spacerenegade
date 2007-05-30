#ifndef SHIP_H
#define SHIP_H

#include <string>
#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"

using std::string;

// General form of ships.  Rather abstract.
class Ship : public Object
{
protected:
	Vec3 direction;

	// "Why store both degrees AND radians?" you may ask.  Because
	// cmath works in radians and OpenGL works in degrees.
	Vec3 degpyr; // Pitch/Yaw/Roll are x/y/z resp. in degrees
	Vec3 radpyr; // like degpyr but in radians.

	double fuel; // Amount of fuel left.
	double ammo; // Amount of ammo left.

	Ship(char *modelName, double fuel, double ammo);

	void recompdir(); // Recompute the direction based on pyr

public:
	virtual ~Ship();

	// Rate functions.
	virtual double roa() const { return 0.005; }; // Rate of Acceleration
	virtual double rod() const { return 0.001; }; // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	virtual string getType() const { return "Ship"; };

	Vec3 getDir() const { return direction; };

	virtual void draw();
	virtual void hits(Object *o);

	// Movement.
	virtual void accelerate()   { velocity += direction * roa(); };
	virtual void decelerate()   { velocity -= direction * rod(); };
	virtual void stabilize();
	virtual void pitchBack();
	virtual void pitchForward();
	virtual void yawLeft();
	virtual void yawRight();
	virtual void rollLeft()  {};
	virtual void rollRight() {};

	// Status stuff.
	virtual double maxHlth() const { return 1000; };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return 100; };
	virtual double getHlth() const { return 1.0 - damage; };
	virtual double getFuel() const { return fuel; };
	virtual double getAmmo() const { return ammo; };
	virtual double hlthPcnt() const { return 1.0 - damage / (double)maxHealth(); };
	virtual double fuelPcnt() const { return fuel / (double)maxFuel(); };
	virtual double ammoPcnt() const { return ammo / (double)maxAmmo(); };
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------ Player's Ship ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Interface for the player's ship.  Very specific to the needs of the player.
class PShip : public Ship
{
	//bool lightRnotL;
	//int lightTime;

public:
	PShip();
	virtual ~PShip();
	virtual string getType() const { return "PShip"; };

	virtual void draw();
	virtual void hits(Object* o);

	virtual void accelerate();
	virtual void decelerate();
	virtual void stabilize();
	virtual void yawLeft();
	virtual void yawRight();
	virtual void rollLeft();
	virtual void rollRight();
	virtual void pitchBack();
	virtual void pitchForward();

	void fire();

	// Rate functions inherited from Ship
	virtual double roa() const { return 0.005; };  // Rate of Acceleration
	virtual double rod() const { return 0.001; };  // Rate of Deceleration
	virtual double ros() const { return 0.95; };  // Rate of Stabilization
	virtual double rot() const { return 0.02; };  // Rate of Turn

	// Status of the ship.
	virtual double maxHlth() const { return 1000; };
	virtual double maxFuel() const { return 10000; };
	virtual double maxAmmo() const { return 100; };
	virtual double hlthPcnt() const { return 1.0 - damage / (double)maxHealth(); };
	virtual double fuelPcnt() const { return fuel / (double)maxFuel(); };
	virtual double ammoPcnt() const { return ammo / (double)maxAmmo(); };
};

#endif

