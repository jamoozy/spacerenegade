#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

enum {CAMERA_MODE_LOOK, CAMERA_MODE_FOLLOW };

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

	Camera();
	void recomputeLook();

public:
	void draw();
	int mode();
	void setMode(int newMode);
	
	// Controls
	void turnUD(double amt);
	void turnLR(double amt);

	void setFocus(const Vec3 &p);
	void setFocus(const Vec3 &p, double dist);
};

void adjustCamera();

#endif
