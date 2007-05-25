#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "objective.h"
#include "bountyobjective.h"
#include <string>
using std::string;

int bountiesKilled = 0;

BountyObjective::BountyObjective(int bountyId, int num) :
	bountyId(bountyId), num(num)
	{
		// create listener
	}

BountyObjective::~BountyObjective() {}

bool BountyObjective::isComplete()
{
	// if listener said bountyId died at least num times, return true
	// else, false
	return (bountiesKilled >= num);
}

string BountyObjective::getDescription()
{
	string response = "";
	string shipName = ""; //getShipName(bountyId);
	if (num == 1) // only one bounty
	{
		if (bountiesKilled == 1) // destroyed bounty
			return (shipName + " destroyed.");
		else // haven't destroyed bounty
			return (shipName + " not yet destroyed.");
	}
	else // several bounties
	{
		// stupid c++ string handling... I'll fix this later

		//response = response.concat(num + " out of ");
		//response.concat (response, num)
		// return (num + " out of " + num + " " + shipName + " destroyed.");
	}
	return "";
}