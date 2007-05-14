#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "object.h"
#include "vec3.h"


///////////////////////////////////////////////////////////////////////////////
// --------------------------- Object Storage ------------------------------ //
///////////////////////////////////////////////////////////////////////////////

// --------- node / leaf / branch structures.
struct Node
{
	Node();
	virtual ~Node();
	virtual void initLeaves();

	virtual void update();
	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
};

struct Branch : public Node
{
	Vec3 split;
	Node *kids[8];

	Branch(int generation, Vec3 split);
	virtual ~Branch();
	virtual void initLeaves();

	virtual void update();
	virtual int getIndex(Object* o) const;
	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
};

struct Leaf : public Node
{
	std::vector<Object*> data;

	Vec3 min, max;  // Bounds of this leaf node.

	Leaf *neighbors[26];   // Pointers to each of the 26 neighbor-leaves.

	Leaf(Vec3 min, Vec3 max);
	virtual ~Leaf();
	virtual void initLeaves();

	virtual void update();
	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
	virtual void remove(Object* o);
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
	void initLeaves();

	void add(Object* o);
	void checkCollisions();
	void update();
};

#endif

