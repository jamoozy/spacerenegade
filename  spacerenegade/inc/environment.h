#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "object.h"


///////////////////////////////////////////////////////////////////////////////
// --------------------------- Object Storage ------------------------------ //
///////////////////////////////////////////////////////////////////////////////

// --------- node / leaf / branch structures.
struct Node
{
	Node();
	virtual ~Node();

	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
};

struct Branch : public Node
{
	Vec3 split;
	Node *kids[8];

	Branch(int generation, Vec3 split);
	virtual ~Branch();

	virtual int getIndex(Object *o) const;
	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
};

struct Leaf : public Node
{
	std::vector<Object*> data;

	Leaf();

	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
};

// ------------- top level part we interact with.
class OctTree
{
	Node* head;

public:
	static const int DEPTH;
	static const double BOUND;

	OctTree();
	virtual ~OctTree();

	void add(Object& o);
	void checkCollisions();
	void update();
};

#endif

