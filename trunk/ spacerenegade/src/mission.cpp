#include "GL/glut.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include "display.h"
#include "menu.h"
#include "mission.h"
#include "objective.h"

using std::string;
using std::cout;
using std::endl;

extern Menu *menu;

void cleanup();

////////////////////////////////////////////////////////////////////////////////
// -------------------------- Mission Definition ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

Mission::Mission(int id) : id(id), title("Title goes here"), briefing("Briefing goes here"),
	numObjs(0), objectives(NULL), reward(0), isComplete(false)
{
	switch (id)
	{
		case 0:
			title = "Help Crooky get his revenge!";
			briefing = "My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plzkthxbai."; // insert \n's?
			numObjs = 1;
			objectives = new BountyObjective*[numObjs];
			objectives[0] = new BountyObjective(0, 5);

			reward = 1000;
			isComplete = false;
			break;

		case 1:
			title = "Help Bumbum get his revenge!";
			briefing = "My name is Bumbum and I want you to kill blues. Blues killed my ma. You kill blues now plzkthxbai."; // insert \n's?
			numObjs = 1;
			objectives = new BountyObjective*[numObjs];
			objectives[0] = new BountyObjective(1, 10);

			reward = 2000;
			isComplete = false;
			break;
	}
}

Mission::~Mission()
{
	for (int i = 0; i < numObjs; i++)
		delete objectives[i];
	delete [] objectives;
}

void Mission::displayMissionBriefing()
{
	drawText(550,450, briefing, Color(1,1,1), false);
}

void Mission::acceptMission()
{
}
