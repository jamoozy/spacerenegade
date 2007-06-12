#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "objective.h"
#include <string>
using std::string;

BountyObjective::BountyObjective() :
	bountyId(0), num(0), bountiesKilled(0) {}

BountyObjective::BountyObjective(int bountyId, int num) :
	bountyId(bountyId), num(num), bountiesKilled(0) {}

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
	string shipName = "SHIPNAME"; //getShipName(bountyId);
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

