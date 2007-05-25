#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include <string>

using std::string;
//#include 

class Objective
{
public:
	// Listener of some sort
	Objective(){}
	bool isComplete();
	string getDescription(); // ex. "2 out of 3 pirates destroyed."
							 //     "Deliver package to Ganymede."
};

#endif
