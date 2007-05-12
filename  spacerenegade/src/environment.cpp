#include "environment.h"



OctTree::OctTree() : head(new Node(1)) {}


OctTree::~OctTree()
{
	delete [] head;
}

void OctTree::registerObject(Object& o)
{
}

void OctTree::checkCollisions()
{
}

void OctTreeupdate()
{
}




int generation;
Node *kids;

Node::Node() : generation(-1), kids(NULL) {}

Node::Node(int generation) : generation(generation), kids(NULL)
{
	if (generation < 4)
	{
		kids = new Node[8];
		for (int i = 0; i < 8; i++)
			kids[i] = Node(generation + 1);
	}
}

Node::~Node()
{
	if (kids != NULL)
		delete [] kids;
}

bool Node::isResident(Object* o)
{
	return false;
}
