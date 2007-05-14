#include <cmath>
#include "environment.h"
#include "vec3.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Node Object ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

// This really does nothing, actually ...
Node::Node() {}

// This does something very important ...
Node::~Node() {}

// Nothing to update.
void Node::update() {}

// This node can't be used, so it can contain nothing.
bool Node::isResident(Object *o) const { return false; }

// Does nothing...  Should be over-written.
void Node::add(Object *o) {}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Branch Object ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Creates a new branch object with the given x,y,z split.
Branch::Branch(int generation, Vec3 split) : split(split)
{
	// --- An Explaination: ---
	//
	// You're probably wondering how the hell I determed where to put the
	// plusses and where to put the minuses and all that, right?  Well
	// I'll tell you.  Take a close look at the numbers.  Do you see the
	// pattern in the Branch constructors formed by the +'s and -'s?
	// It's a binary pattern.  If you take away everything except the
	// +'s and -'s, then replace all +'s with a 0 and all -'s with a 1,
	// you'll get the binary representation of the numbers 0 - 7.  This
	// allows for (somewhat) fast calculation of index when the time
	// comes.  For a concrete example, see the getIndex() method.

	if (generation < pow(2, OctTree::DEPTH))
	{
		int nextGen = generation + generation;
		double inc = OctTree::BOUND / nextGen;

		kids[0] = new Branch(nextGen, Vec3(split.x() + inc, split.y() + inc, split.z() + inc));
		kids[1] = new Branch(nextGen, Vec3(split.x() + inc, split.y() + inc, split.z() - inc));
		kids[2] = new Branch(nextGen, Vec3(split.x() + inc, split.y() - inc, split.z() + inc));
		kids[3] = new Branch(nextGen, Vec3(split.x() + inc, split.y() - inc, split.z() - inc));
		kids[4] = new Branch(nextGen, Vec3(split.x() - inc, split.y() + inc, split.z() + inc));
		kids[5] = new Branch(nextGen, Vec3(split.x() - inc, split.y() + inc, split.z() - inc));
		kids[6] = new Branch(nextGen, Vec3(split.x() - inc, split.y() - inc, split.z() + inc));
		kids[7] = new Branch(nextGen, Vec3(split.x() - inc, split.y() - inc, split.z() - inc));
	}
	else
	{
		double inc = OctTree::BOUND / generation;

		kids[0] = new Leaf(Vec3(split.x(), split.y(), split.z()), Vec3(split.x()+inc, split.y()+inc, split.z()+inc));
		kids[1] = new Leaf(Vec3(split.x(), split.y(), split.z()-inc), Vec3(split.x()+inc, split.y()+inc, split.z()));
		kids[2] = new Leaf(Vec3(split.x(), split.y()-inc, split.z()), Vec3(split.x()+inc, split.y(), split.z()+inc));
		kids[3] = new Leaf(Vec3(split.x(), split.y()-inc, split.z()-inc), Vec3(split.x()+inc, split.y(), split.z()));
		kids[4] = new Leaf(Vec3(split.x()-inc, split.y(), split.z()), Vec3(split.x(), split.y()+inc, split.z()+inc));
		kids[5] = new Leaf(Vec3(split.x()-inc, split.y(), split.z()-inc), Vec3(split.x(), split.y()+inc, split.z()));
		kids[6] = new Leaf(Vec3(split.x()-inc, split.y()-inc, split.z()), Vec3(split.x(), split.y(), split.z()+inc));
		kids[7] = new Leaf(Vec3(split.x()-inc, split.y()-inc, split.z()-inc), Vec3(split.x(), split.y(), split.z()));
	}
}

// Delete the kids.
Branch::~Branch()
{
	for (int i = 0; i < 8; i++)
		if (kids[i] != NULL)
			delete kids[i];
}

// Update all the children.
void Branch::update()
{
	for (int i = 0; i < 8; i++)
		kids[i]->update();
}

// Gets the index of the child the passed object is in.
int Branch::getIndex(Object *o) const
{
	int i = 0;
	if (o->getPos().x() < split.x())
		i += 4;
	if (o->getPos().y() < split.y())
		i += 2;
	if (o->getPos().z() < split.z())
		i += 1;

	return i;
}

// Determine if the passed object pointer points to an object in this tree.
bool Branch::isResident(Object* o) const
{
	return kids[getIndex(o)]->isResident(o);
}

// Add an object to the tree.  If it's already in the node, it will not be added.
void Branch::add(Object* o)
{
	kids[getIndex(o)]->add(o);
}



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- Leaf Object --------------------------------- //
////////////////////////////////////////////////////////////////////////////////

Leaf::Leaf(Vec3 min, Vec3 max) : min(min), max(max) {}

Leaf::~Leaf()
{
	for (unsigned int i = 0; i < data.size(); i++)
		delete data[i];
}

bool Leaf::isResident(Object *o) const
{
	for (unsigned int i = 0; i < data.size(); i++)
		if (data[i] == o)
			return true;

	return false;
}

// Update all of the members of this leaf node.
void Leaf::update()
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		data[i]->draw();
		data[i]->checkResidence();
	}
}

// Add the given object to this Leaf.  If it's already
// resident, do nothing.
void Leaf::add(Object *o)
{
	if (!isResident(o))
	{
		data.push_back(o);
		o->setResidence(this);
	}
}

// Remove the given object from this leaf.  If it's not
// resident, do nothing.
void Leaf::remove(Object *o)
{
	for (unsigned int i = 0; i < data.size(); i++)
		if (data[i] == o)
			data.erase(data.begin()+i);
}



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- OctTree Object ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

const int OctTree::DEPTH = 4;

const double OctTree::BOUND = 1000;

OctTree::OctTree() : head(new Branch(1 , Vec3(0,0,0))) {}


OctTree::~OctTree()
{
	delete head;
}

void OctTree::add(Object* o)
{
	head->add(o);
}

void OctTree::update()
{
	head->update();
}

