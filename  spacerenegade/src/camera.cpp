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
Camera::Camera() : pos(0,10,0), look(0,0,-1), up(0,1,0), _mode(CAMERA_MODE_FOLLOW) {}

// Draw is just a call to gluLookAt.
void Camera::draw()
{
	gluLookAt(pos.x(), pos.y(), pos.z(),
	          pos.x() + look.x(), pos.y() + look.y(), pos.z() + look.z(),
			  up.x(), up.y(), up.z());
}

int Camera::mode()
{
	return _mode;
}

void Camera::setMode(int newMode)
{
	_mode = newMode;
}


// Alter the camera as needed and put its changes into effect.
void adjustCamera()
{
	Camera::getCamera()->draw();
}
