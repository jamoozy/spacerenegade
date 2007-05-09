#include <GL/glut.h>
#include "camera.h"


Camera *Camera::camera = NULL;

Camera *Camera::getCamera()
{
	if (camera == NULL)
		camera = new Camera();

	return camera;
}

void Camera::cleanUp()
{
	if (camera != NULL)
	{
		delete camera;
		camera = NULL;
	}
}

// Initialize a boring camera looking down the z-axis.
Camera::Camera() : pos(0,-.5,0), lookAt(0,0,-1), up(0,1,0) {}

// Draw is just a call to gluLookAt.
void Camera::draw()
{
	gluLookAt(pos.x(), pos.y(), pos.z(),
	          lookAt.x(), lookAt.y(), lookAt.z(),
			  up.x(), up.y(), up.z());
}


// Alter the camera as needed and put its changes into effect.
void adjustCamera()
{
	Camera::getCamera()->draw();
}