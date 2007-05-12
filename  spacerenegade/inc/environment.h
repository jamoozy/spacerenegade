#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"


///////////////////////////////////////////////////////////////////////////////
// --------------------------- Object Storage ------------------------------ //
///////////////////////////////////////////////////////////////////////////////

struct Node
{
	int generation;
	Node *kids;

	Node();
	Node(int generation);
	~Node();

	bool isResident(Object* o);
};

class OctTree
{
	Node* head;

public:
	OctTree();
	~OctTree();

	void registerObject(Object& o);
	void checkCollisions();
	void update();
};

#endif
