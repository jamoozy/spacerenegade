#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

enum { CAMERA_MODE_DEFAULT,     // Determined by other aspects.
       CAMERA_MODE_FOLLOW,      // Follows the ship.
	   CAMERA_MODE_LOOK,        // Mouse-controlled Look
	   CAMERA_MODE_BEHIND_10};  // Behind and up 10 units.

class Camera
{
	static Camera *camera;

public:
	static double DEFAULT_DIST;
	static Camera *getCamera();
	static void cleanUp();

private:
	Vec3 pos;
	Vec3 lookat;  // direction, not point.
	Vec3 up;

	double theta, phi, lookinc;  // Theta is around the up vector, phi above the horizon.

	int _mode;
	int _defaultMode;

	Camera();
	void recomputeLook();

public:
	void draw();
	int mode();
	void setMode(int newMode);
	void setDefaultMode(int newMode);
	void toggleDefault();
	
	// Controls
	void turnUD(double amt);
	void turnLR(double amt);
	void setFocus(const Vec3 &p);
	void setFocus(const Vec3 &p, double dist);

	Vec3 getDir() { return lookat - pos; };
};

void adjustCamera();

#endif
