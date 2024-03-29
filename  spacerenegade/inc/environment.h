#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "object.h"
#include "vec3.h"

using std::vector;


///////////////////////////////////////////////////////////////////////////////
// --------------------------- Object Storage ------------------------------ //
///////////////////////////////////////////////////////////////////////////////

// --------- node / leaf / branch structures.
struct Node
{
	Node() {};
	virtual ~Node() {};
	virtual void initLeaves() {};

	virtual void update() {};
	virtual void draw(int pass) {};
	virtual bool isResident(Object* o) const { return false; };
	virtual void add(Object* o) {};
};

struct Branch : public Node
{
	Vec3 split;
	Node *kids[8];

	Branch(int generation, int maxDepth, Vec3 split);
	virtual ~Branch();
	virtual void initLeaves();

	virtual void update();
	virtual void draw(int pass);
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
	virtual void draw(int pass);
	virtual bool isResident(Object* o) const;
	virtual void add(Object* o);
	virtual void remove(Object* o);
	virtual const Vec3& getMin() const { return minBound; };
	virtual const Vec3& getMax() const { return maxBound; };
	virtual Vec3 size() const { return (maxBound - minBound); };
	virtual Vec3 center() const { return (maxBound + minBound) / 2; };
};

// ------------- top level part we interact with.
class OctTree
{
	Node* head;
	unsigned int gridDim;
	Leaf**** grid;  // 3-dim array of Leaf pointers

public:
	static const double BOUND;

	OctTree(int maxDepth);
	void initLeaves();
	virtual ~OctTree();
	Vec3 indexOf(Leaf *l);

	void add(Object* o);
	void checkCollisions();
	void update();
	void draw(int pass);
	void getArea(const Vec3& pos, double radius, vector<Object*>& objs);
};

#endif

