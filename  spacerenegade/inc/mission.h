#ifndef MISSION_H
#define MISSION_H

#include "objective.h"

class Mission
{
private:
	int id;
	bool isComplete;
	string title; // Headline of mission ("Help Crooky get his revenge!")
	string briefing; // Detailed explanation of missions as told by client
	Objective **objectives;
	int reward; // ex. 1000 credits

	//bool isComplete; // all objectives have been met, and player should go to finishPlanet
	//boolean hasFailed; // one or more conditions have been failed (such as allowing a ship to die)
	// EventListener listens for objective-specific events (such as # of bounties killed)

	// Desired, but not achievable:
	//   Location startPlanet, finishPlanet; // Start & Finish locations (for delivery, escort, etc)
	//   Time deadline;

public:
	Mission();
	virtual ~Mission();
	Mission(int id); // probably only one or two missions in demo
	bool isCompleted();
	//bool hasFailed(); // won't use conditions in the demo
	string getTitle();
	string getBriefing();
	int getReward();
	//Objective getObjectives();
	void getObjectives(Object **objs);
};

#endif