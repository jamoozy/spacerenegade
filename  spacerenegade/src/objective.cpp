#include <iostream>
#include "GL/glut.h"
#include "environment.h"
#include "vec3.h"
#include "objective.h"
#include <string>
#include <sstream>
using namespace std;

BountyObjective::BountyObjective() :
	shipName("SHIPNAME"), bountyId(0), num(0), bountiesKilled(0) {}

BountyObjective::BountyObjective(string targetShipNames, int bountyId, int num) :
	shipName(targetShipNames), bountyId(bountyId), num(num), bountiesKilled(0) {}

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
	ostringstream oss;

	//string shipName = "SHIPNAME"; //getShipName(bountyId);
	if (num == 1) // only one bounty
	{
		if (bountiesKilled == 1) // destroyed bounty
			oss << shipName << " destroyed.";
		else // haven't destroyed bounty
			//oss << shipName << " not yet destroyed.";
			oss << "Mission Complete!";
	}
	else // several bounties
	{
		if(bountiesKilled < num)
		{
			oss << bountiesKilled << " out of " << num;
			oss << response << " " << shipName << " destroyed.";
		}
		else
			oss << "Mission Complete!";
	}
	return oss.str();
}

