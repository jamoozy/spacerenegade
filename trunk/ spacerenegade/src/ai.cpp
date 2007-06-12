#include "object.h"
#include "ship.h"
#include "factionInfo.h"
#include "vec3.h"
#include "environment.h"
#include "ai.h"
#include "astarsearch.h"
#include <vector>

#ifndef M_PI
	#define M_PI 3.14159265358979
#endif

extern PShip *playerShip;
extern OctTree *env;

using namespace std;

ShipAI::ShipAI(Ship* myShip, FactionInfo* myFaction) : pilotedShip(myShip),
		myFactionInfo(myFaction), engagementRadius(400.0), currentMode(WAITING),
		target(NULL), destination(Vec3(0.0, 0.0, 0.0)), proximityDistance(10.0),
		fireCoolDown(0), updateCountDown(0)
{}

void ShipAI::update()
{
	switch(currentMode)
	{
		// Hold Position until something new happens
		case WAITING:
			if((myFactionInfo->getAttitude(PLAYER) == ENEMY) 
				&& pilotedShip->getPos().distance(playerShip->getPos()) <= engagementRadius)
			{
				target = playerShip;
				currentMode = ATTACK;
			}
			else 
			{
				currentMode = PATROL;
				destination = Vec3(rr(-250, 250), rr(-250, 250), rr(-250, 250));
			}
			break;
		case PATROL:
			if((myFactionInfo->getAttitude(PLAYER) == ENEMY) 
				&& pilotedShip->getPos().distance(playerShip->getPos()) <= engagementRadius)
			{
				target = playerShip;
				currentMode = ATTACK;
			}
			else 
			{
				// If we get nearby, move on
				if(pilotedShip->getPos().distance(destination) <= proximityDistance * 2)
				{
					destination = Vec3(rr(-250, 250), rr(-250, 250), rr(-250, 250));
				}

				if(updateCountDown <= 0)
				{
					AStarSearch astar(pilotedShip->getPos(), destination, proximityDistance, 
						pilotedShip->getRadius(), 0.0);
					// Only find the shortest path again if the path to a waypoint is blocked
					if(!((path.size() > 2) && astar.pathIsClear(path.at(0), path.at(1))))
					{
						vector<Vec3> shortestPath = astar.findShortestPath();
						// Copy to path
						path.clear();
						for(unsigned int i = 0; i < shortestPath.size(); i++)
						{
							Vec3 temp = shortestPath.at(i);
							path.push_back(shortestPath.at(i));
						}
					}
					updateCountDown = 60;
				}
				else
					updateCountDown--;

				Vec3 nextWayPoint = path.at(1);
				goTo(nextWayPoint);
			}
			break;
		case ATTACK:
			Vec3 currentPos = pilotedShip->getPos();
			double otherRadius;
			if(target != NULL)
			{
				destination = target->getPos();
				otherRadius = target->getRadius();
				Vec3 targetPos = playerShip->getPos();
				// Break off pursuit
				if(currentPos.distance(targetPos) > engagementRadius)
				{
					target = NULL;
					currentMode = WAITING;
					break;
				}
			}
			else
				otherRadius = 0;
			if(updateCountDown <= 0)
			{
				AStarSearch astar(currentPos, destination, proximityDistance, 
					pilotedShip->getRadius(), otherRadius);
				// Only find the shortest path again if the path to a waypoint is blocked
				if(!((path.size() > 2) && astar.pathIsClear(path.at(0), path.at(1))))
				{
					vector<Vec3> shortestPath = astar.findShortestPath();
					// Copy to path
					path.clear();
					for(unsigned int i = 0; i < shortestPath.size(); i++)
					{
						Vec3 temp = shortestPath.at(i);
						path.push_back(shortestPath.at(i));
					}
				}
				updateCountDown = 60;
			}
			else
				updateCountDown--;

			Vec3 nextWayPoint = path.at(1);
			goTo(nextWayPoint);
			// Fire if goign straight for the player
			if(path.size() == 2)
			{
				if(fireCoolDown == 0)
				{
					pilotedShip->fire();
					fireCoolDown = 30;
				}
				else
					fireCoolDown--;

			}
			break;
	};

}

bool ShipAI::goTo(const Vec3& pos)
{	
	// Find the normalized velocity and path vectors
	Vec3 meToPos = pos - pilotedShip->getPos();
	Vec3 temp = pilotedShip->getPos();
	// Return true and slow down if the ship has arrived
	double closeDistance = proximityDistance;
	if(target != NULL && (target->getPos() == pilotedShip->getPos()))
		closeDistance += pilotedShip->getRadius() + target->getRadius();
	if(meToPos.norm() <= closeDistance)
	{
		pilotedShip->stabilize();
		return true;
	}
	
	//meToPos.normalize();
	Vec3 direction = pilotedShip->getVel();
	//direction.normalize();
/*
	Vec3 forward(0, 0, 1);
	Vec3 up(0, 1, 0);
	// Rotate and project vectors onto the XY-plane to determine yaw amount
	Vec3 meToPosY = meToPos;
	meToPosY.setY(0.0);
	meToPosY.normalize();
	Vec3 directionY = direction;
	directionY.setY(0.0);
	directionY.normalize();

	// Determine if ship needs to be turned left or right
	double angle = meToPosY.angleBetween(directionY);
	double crossProduct = (meToPosY ^ directionY).y();

	// Turn left
	if(crossProduct < 0 && angle >= pilotedShip->rot())
		left();
	// Turn right
	else if(angle >= pilotedShip->rot())
		right();

	// Project vectors onto the XY-plane to determine pitch amount
	Vec3 meToPosZ = meToPos;
	meToPosZ.setZ(0.0);
	meToPosZ.normalize();
	Vec3 directionZ = direction;
	directionZ.setZ(0.0);
	directionZ.normalize();

	// Determine if ship needs to be pitched up or down
	angle = meToPosZ.angleBetween(directionZ);
	crossProduct = (meToPosZ ^ directionZ).z();

	// Go down
	if(crossProduct < 0 && angle >= pilotedShip->rot())
		down();
	// Go up
	else if(angle >= pilotedShip->rot())
		up();
*/
	// Accelerate
	double speed = pilotedShip->getVel().norm();
	// Determine if there is a need to slow down
	double t = speed / pilotedShip->rod();
	if((speed * t - 0.5 * pilotedShip->rod() * pow(t, 2)) >=(meToPos.norm() 
			- closeDistance))
		pilotedShip->decelerate();
	else
		pilotedShip->accelerate();

	// Insure ship is looking in the right direction	
	meToPos.normalize();
	pilotedShip->setDir(meToPos);
	pilotedShip->eliminateOldInertia();

	return false;
}

void ShipAI::left()
{
	// Get the number of radians rotated, counterclockwise
	double angle = pilotedShip->getRadiansRotated();

	// 45 degrees to -45 degrees
	if((angle <= M_PI / 4.0) || (angle > 7.0 * M_PI / 4.0))
		pilotedShip->yawLeft();
	// 45 degrees to 135 degrees
	else if(angle <= 3.0 * M_PI / 4.0)
		pilotedShip->pitchBack();
	// 135 degrees to -135 degrees
	else if(angle <= 5.0 * M_PI / 4.0)
		pilotedShip->yawRight();
	// -45 degrees to -135 degrees
	else
		pilotedShip->pitchForward();
}

void ShipAI::right()
{
	// Get the number of radians rotated, counterclockwise
	double angle = pilotedShip->getRadiansRotated();

	// 45 degrees to -45 degrees
	if((angle <= M_PI / 4.0) || (angle > 7.0 * M_PI / 4.0))
		pilotedShip->yawRight();
	// 45 degrees to 135 degrees
	else if(angle <= 3.0 * M_PI / 4.0)
		pilotedShip->pitchForward();
	// 135 degrees to -135 degrees
	else if(angle <= 5.0 * M_PI / 4.0)
		pilotedShip->yawLeft();
	// -45 degrees to -135 degrees
	else
		pilotedShip->pitchBack();
}

void ShipAI::up()
{
	// Get the number of radians rotated, counterclockwise
	double angle = pilotedShip->getRadiansRotated();

	// 45 degrees to -45 degrees
	if((angle <= M_PI / 4.0) || (angle > 7.0 * M_PI / 4.0))
		pilotedShip->pitchBack();
	// 45 degrees to 135 degrees
	else if(angle <= 3.0 * M_PI / 4.0)
		pilotedShip->yawRight();
	// 135 degrees to -135 degrees
	else if(angle <= 5.0 * M_PI / 4.0)
		pilotedShip->pitchForward();
	// -45 degrees to -135 degrees
	else
		pilotedShip->yawLeft();
}

void ShipAI::down()
{
	// Get the number of radians rotated, counterclockwise
	double angle = pilotedShip->getRadiansRotated();

	// 45 degrees to -45 degrees
	if((angle <= M_PI / 4.0) || (angle > 7.0 * M_PI / 4.0))
		pilotedShip->pitchForward();
	// 45 degrees to 135 degrees
	else if(angle <= 3.0 * M_PI / 4.0)
		pilotedShip->yawLeft();
	// 135 degrees to -135 degrees
	else if(angle <= 5.0 * M_PI / 4.0)
		pilotedShip->pitchBack();
	// -45 degrees to -135 degrees
	else
		pilotedShip->yawRight();
}

