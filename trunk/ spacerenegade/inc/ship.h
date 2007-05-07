#ifndef SHIP_H
#define SHIP_H


#include "object.h"
#include "vec3.h"


class Ship : public Object
{
private:
	Vec3 direction;

	// "Why store both degrees AND radians?" you may ask.  Because
	// cmath works in radians and OpenGL works in degrees.
	Vec3 degpyr; // Pitch/Yaw/Roll are x/y/z resp. in degrees
	Vec3 radpyr; // like degpyr but in radians.

	// Rates.
	double roa;  // Rate of Acceleration
	double ros;  // Rate of Stabilization
	double rot;  // Rate of Turn

	void recompdir(); // Recompute the direction based on pyr

public:
	Ship();
	void draw();
	void accelerate();
	void decelerate();
	void stabilize();
	void yawLeft();
	void yawRight();
	void rollLeft();
	void rollRight();
	void pitchBack();
	void pitchForward();

	void setAt(double x, double y, double z);
};

#endif

