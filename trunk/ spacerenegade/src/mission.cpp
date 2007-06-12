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
			briefing = "My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. My name is Crooky and I want you to kill reds. Reds killed my pa. You kill reds now plz kthxbai. ";
			numObjs = 1;
			objectives = new BountyObjective*[numObjs];
			objectives[0] = new BountyObjective(0, 5);

			reward = 1000;
			isComplete = false;
			break;

		case 1:
			title = "Help Bumbum get his revenge!";
			briefing = "My name is Bumbum and I want you to kill blues. Blues killed my ma. You kill blues now plz kthxbai. ";
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
	drawText(515,725, insertNewLines(briefing, 41), Color(1,1,1), false);
}

void Mission::acceptMission()
{

}

// Automatically inserts "\" at every width'th character,
// formatting to break at spaces.
string Mission::insertNewLines(string b, int width)
{
	string newB = "";
	string chunk;
	unsigned int k = 0;
	int spaceIndex;
	while (k < b.length())
	{
		if ((k + width) > b.length()) // end of briefing
		{
			newB.append(b.substr(k, b.length()));
			k += width;
		}
		else
		{
			chunk = b.substr(k,width+1);
			spaceIndex = chunk.find_last_of(" ");
			chunk = chunk.substr(0, spaceIndex);
			newB.append(chunk + "\n");
			k += spaceIndex + 1;
		}
	}
	return newB;
}
