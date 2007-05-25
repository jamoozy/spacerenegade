#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include <string>

using std::string;
//#include 

class Objective
{
protected:
	// Listener of some sort

public:
	Objective(){}
	bool isComplete();
	string getDescription();
};

#endif
