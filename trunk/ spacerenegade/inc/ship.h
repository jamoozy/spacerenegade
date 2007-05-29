#ifndef SHIP_H
#define SHIP_H

#include "Model_3DS.h"
#include "object.h"
#include "vec3.h"


class Ship : public Object
{
private:
	Vec3 direction;

	//bool lightRnotL;
	//int lightTime;

	// "Why store both degrees AND radians?" you may ask.  Because
	// cmath works in radians and OpenGL works in degrees.
	Vec3 degpyr; // Pitch/Yaw/Roll are x/y/z resp. in degrees
	Vec3 radpyr; // like degpyr but in radians.

	// Rates.
	double roa;  // Rate of Acceleration
	double rod;  // Rate of Deceleration
	double ros;  // Rate of Stabilization
	double rot;  // Rate of Turn

	double fuel; // Amount of fuel left.
	double ammo; // Amount of ammo left.

	void recompdir(); // Recompute the direction based on pyr

public:
	Ship();
	virtual ~Ship();
	virtual std::string getType() const { return "Ship"; };

	Vec3 getDir() const { return direction; };
	virtual void draw();

	void accelerate();
	void decelerate();
	void stabilize();
	void yawLeft();
	void yawRight();
	void rollLeft();
	void rollRight();
	void pitchBack();
	void pitchForward();

	void fire();
	virtual void hits(Object *o);
	virtual int maxHealth() const { return 1000; };
	virtual int maxFuel() const { return 10000; };
	virtual int maxAmmo() const { return 100; };
	virtual double getHealth() const { return 1.0 - damage / (double)maxHealth(); };
	virtual double getFuel() const { return fuel / (double)maxFuel(); };
	virtual double getAmmo() const { return ammo / (double)maxAmmo(); };
};

#endif

