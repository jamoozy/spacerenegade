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

	Vec3 minBound, maxBound;  // Bounds of this leaf node.

	Leaf *checkedNeighbors[13];   // Pointers to each of the 13 neighbor-leaves this leaf is responsible for comparing collisions against.
	Leaf *unCheckedNeighbors[13]; // Pointers to the rest of the neighbors (for completeness).

	Leaf(const Vec3 minBound, const Vec3 maxBound);
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
	void getArea(const Vec3& pos, double radius, Object **objs, int& numObjs);
};

#endif

