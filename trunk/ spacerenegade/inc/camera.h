#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class Camera
{
	static Camera *camera;

public:
	static Camera *getCamera();
	static void cleanUp();

private:
	Vec3 pos;
	Vec3 lookAt;
	Vec3 up;

	Camera();

public:
	void draw();
};

void adjustCamera();

#endif