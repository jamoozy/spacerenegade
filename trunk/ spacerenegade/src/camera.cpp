#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "ship.h"
#include "input.h"
#include "camera.h"

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

#ifndef HALF_PI
	#define HALF_PI 1.570796326794895
#endif

#ifndef TWO_PI
	#define TWO_PI 6.28318530717958
#endif

extern Ship *playerShip;



///////////////////////////////////////////////////////////////////////////////
// --------------------------- Camera Definitions -------------------------- //
///////////////////////////////////////////////////////////////////////////////

// ----- Static parts ------------

Camera *Camera::camera = NULL;

double Camera::DEFAULT_DIST = 50;

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

// ---------- Object parts ----------------

// Initialize a boring camera looking down the z-axis.
Camera::Camera() : pos(0,0,0), lookat(0,0,-50), up(0,1,0),
                   _mode(CAMERA_MODE_FOLLOW),
				   theta(M_PI), phi(0), lookinc(.005)
{
	recomputeLook();
}

// Draw is just a call to gluLookAt.
void Camera::draw()
{
	gluLookAt(pos.x(), pos.y(), pos.z(),
	          lookat.x(), lookat.y(), lookat.z(),
			  up.x(), up.y(), up.z());
}

void Camera::recomputeLook()
{
	std::cout << "LookAt: " << lookat.str() << std::endl;
	Vec3 look = Vec3(sin(theta) * cos(phi), sin(phi), cos(theta) * cos(phi));

	double dist = (pos - lookat).norm();
	pos = lookat + look * (-dist);
}

void Camera::turnUD(int amt)
{
	double newphi = phi - lookinc * amt;
	
	if (newphi > -HALF_PI && newphi < HALF_PI)
	{
		phi = newphi;
		recomputeLook();
	}
}

void Camera::turnLR(int amt)
{
	std::cout << "old: " << theta << std::endl;
	theta += lookinc * amt;
	std::cout << "new: " << theta << std::endl;
	recomputeLook();
}

int Camera::mode()
{
	return _mode;
}

void Camera::setMode(int newMode)
{
	_mode = newMode;
}

// Start looking at the point and remain stationary.
void Camera::setFocus(Vec3 &p)
{
	lookat = p;
	Vec3 look = lookat - pos;
	look.normalize();
	phi = asin(look.y());
	theta = atan(look.x()/look.z());
	if (look.z() < 0)
		theta += M_PI;
	else if (theta < 0)
		theta += TWO_PI;
}

// Start looking at the point and remain "dist" away from it.
void Camera::setFocus(Vec3 &p, double dist)
{
	lookat = p;
	Vec3 look = lookat - pos;
	look.normalize();
	phi = asin(look.y());
	theta = atan(look.x()/look.z());
	if (look.z() < 0)
		theta += M_PI;
	else if (theta < 0)
		theta += TWO_PI;

	pos = lookat + (look * (-dist));
}


// Alter the camera as needed and put its changes into effect.
void adjustCamera()
{
	Camera::getCamera()->setFocus(playerShip->getPos(), Camera::DEFAULT_DIST);

	if (Camera::getCamera()->mode() == CAMERA_MODE_LOOK)
	{
		Camera::getCamera()->turnLR(Mouse::getMouse()->getDiffX());
		Camera::getCamera()->turnUD(Mouse::getMouse()->getDiffY());
		Mouse::getMouse()->clearDiffs();
	}
	
	Camera::getCamera()->draw();
}
