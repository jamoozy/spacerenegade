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

	void recompdir(); // Recompute the direction based on pyr

public:
	Ship();
	virtual ~Ship();
	Vec3 getDir();
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
	//virtual void hits(Object *o);
};

#endif

