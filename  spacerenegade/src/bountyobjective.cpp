#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "objective.h"
#include "bountyobjective.h"
#include <string>
using std::string;

int bountiesKilled = 0;

BountyObjective::BountyObjective()
{
	bountyId = 0;
	num = 0;
}

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
	string response;
	string shipName = ""; //getShipName(bountyId);
	if (num == 1) // only one bounty
	{
		if (bountiesKilled == 1) // destroyed bounty
			response = shipName + " destroyed.";
		else // haven't destroyed bounty
			response = shipName + " not yet destroyed.";
	}
	else // several bounties
	{
		response = bountiesKilled + " out of " + num;
		response += " " + shipName + " destroyed.";
	}
	return response;
}