#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

enum {CAMERA_MODE_LOOK, CAMERA_MODE_FOLLOW };

class Camera
{
	static Camera *camera;

public:
	static Camera *getCamera();
	static void cleanUp();

private:
	Vec3 pos;
	Vec3 look;  // direction, not point.
	Vec3 up;

	int _mode;

	Camera();

public:
	void draw();
	int mode();
	void setMode(int newMode);
};

void adjustCamera();

#endif

