#include <iostream>
#include <cmath>
#include "environment.h"
#include "vec3.h"

using namespace std;
extern OctTree *env;


////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Node Object ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

// This really does nothing, actually ...
Node::Node() {}

// This, too, does nothing.
void Node::initLeaves() {}

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

	if (generation < pow(2.0, (OctTree::DEPTH)))
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

// Tell all the child-nodes to initialize the leaves.
void Branch::initLeaves()
{
	for (int i = 0; i < 8; i++)
		kids[i]->initLeaves();
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

Leaf::Leaf(Vec3 minBound, Vec3 maxBound) : minBound(minBound), maxBound(maxBound) {}

void Leaf::initLeaves()
{
	// ---- Another explaination: ----
	//
	// I'm using an "anchor" to add objects just slightly out of the range
	// of this leaf, find where they landed, and that will be a neighbor.
	// Think of fishing - once the lure hits the bottom a fish grabs it.
	// Get the fish and re-cast the lure in a different spot.

	Vec3 center((maxBound+minBound)/2);
	double diff = maxBound.x() - center.x() + 1;

	Object *anchor = new Object("");

	anchor->setAt(center+Vec3(-diff,-diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[0] = anchor->getResidence();
	else
		checkedNeighbors[0] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,-diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[1] = anchor->getResidence();
	else
		checkedNeighbors[1] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,-diff,+diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[2] = anchor->getResidence();
	else
		checkedNeighbors[2] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,0,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[3] = anchor->getResidence();
	else
		checkedNeighbors[3] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,0,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[4] = anchor->getResidence();
	else
		checkedNeighbors[4] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,0,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[5] = anchor->getResidence();
	else
		checkedNeighbors[5] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[6] = anchor->getResidence();
	else
		checkedNeighbors[6] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[7] = anchor->getResidence();
	else
		checkedNeighbors[7] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(-diff,diff,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[8] = anchor->getResidence();
	else
		checkedNeighbors[8] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(0,-diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[9] = anchor->getResidence();
	else
		checkedNeighbors[9] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(0,-diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[10] = anchor->getResidence();
	else
		checkedNeighbors[10] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(0,-diff,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[11] = anchor->getResidence();
	else
		checkedNeighbors[11] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center+Vec3(0,0,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		checkedNeighbors[12] = anchor->getResidence();
	else
		checkedNeighbors[12] = NULL;
	anchor->getResidence()->remove(anchor);
	
	anchor->setAt(center-Vec3(-diff,-diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[0] = anchor->getResidence();
	else
		unCheckedNeighbors[0] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,-diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[1] = anchor->getResidence();
	else
		unCheckedNeighbors[1] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,-diff,+diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[2] = anchor->getResidence();
	else
		unCheckedNeighbors[2] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,0,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[3] = anchor->getResidence();
	else
		unCheckedNeighbors[3] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,0,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[4] = anchor->getResidence();
	else
		unCheckedNeighbors[4] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,0,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[5] = anchor->getResidence();
	else
		unCheckedNeighbors[5] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[6] = anchor->getResidence();
	else
		unCheckedNeighbors[6] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[7] = anchor->getResidence();
	else
		unCheckedNeighbors[7] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(-diff,diff,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[8] = anchor->getResidence();
	else
		unCheckedNeighbors[8] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(0,-diff,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[9] = anchor->getResidence();
	else
		unCheckedNeighbors[9] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(0,-diff,0));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[10] = anchor->getResidence();
	else
		unCheckedNeighbors[10] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(0,-diff,diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[11] = anchor->getResidence();
	else
		unCheckedNeighbors[11] = NULL;
	anchor->getResidence()->remove(anchor);

	anchor->setAt(center-Vec3(0,0,-diff));
	env->add(anchor);
	if (anchor->getResidence() != this)
		unCheckedNeighbors[12] = anchor->getResidence();
	else
		unCheckedNeighbors[12] = NULL;
	anchor->getResidence()->remove(anchor);

	delete anchor;
}

Leaf::~Leaf()
{
	for (unsigned int i = 0; i < data.size(); i++)
		delete data[i];
}

bool Leaf::isResident(Object *o) const
{
	return (this == o->getResidence());

//	for (unsigned int i = 0; i < data.size(); i++)
//		if (data[i] == o)
//			return true;
//
//	return false;
}

// Update all of the members of this leaf node.
void Leaf::update()
{
	using std::cout;
	using std::endl;

//	cout << "entered Leaf::update()" << endl;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i]->checkResidence())
		{
			// Check all the dudes in this node.
			for (unsigned int j = i+1; j < data.size(); j++) {
				if (data[i]->collidesWith(data[j])) {
					data[i]->hits(data[j]);
					data[j]->hits(data[i]);
				}
			}
			
			// Check all the dudes in the neighboring nodes.
			for (int j = 0; j < 13; j++) {
				if (checkedNeighbors[j] != NULL) {
					for (unsigned int k = 0; k < checkedNeighbors[j]->data.size(); k++) {
						if (data[i]->collidesWith(checkedNeighbors[j]->data[k])) {
							data[i]->hits(checkedNeighbors[j]->data[k]);
							checkedNeighbors[j]->data[k]->hits(data[i]);
						}
					}
				}
			}
		}
		data[i]->draw();
	}
//	cout << "leaving Leaf::update()" << endl;
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
		{
			data[i]->setResidence(NULL);
			data.erase(data.begin()+i);
		}
}



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- OctTree Object ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

const int OctTree::DEPTH = 3;

const double OctTree::BOUND = 1000;

OctTree::OctTree() : head(new Branch(1 , Vec3(0,0,0))) {}

void OctTree::initLeaves() { head->initLeaves(); }

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

void OctTree::getArea(const Vec3& pos, double radius, Object **objs, int& numObjs)
{
	double radius2 = radius * radius;

	Object o("");
	o.setAt(pos);
	head->add(&o);

	Leaf *l = o.getResidence();
	l->remove(&o);

	numObjs = 0;   // Also serves as the numObjs slot in objs to assign a value.

	for (unsigned int i = 0; i < l->data.size(); i++){
		if ((l->data[i]->getPos() - pos) * (l->data[i]->getPos() - pos) < radius2){
			objs[numObjs++] = l->data[i];
		}//if
	}//for

	for (int i = 0; i < 13; i++){
		if (l->checkedNeighbors[i] != NULL){
			for (unsigned int j = 0; j < l->checkedNeighbors[i]->data.size(); j++){
				if ((l->checkedNeighbors[i]->data[j]->getPos() - pos) * (l->checkedNeighbors[i]->data[j]->getPos() - pos) < radius2){
					objs[numObjs++] = l->checkedNeighbors[i]->data[j];
				}//if
			}//for
		}//if
	}//for

	for (int i = 0; i < 13; i++){
		if (l->unCheckedNeighbors[i] != NULL){
			for (unsigned int j = 0; j < l->unCheckedNeighbors[i]->data.size(); j++){
				if ((l->unCheckedNeighbors[i]->data[j]->getPos() - pos) * (l->unCheckedNeighbors[i]->data[j]->getPos() - pos) < radius2){
					objs[numObjs++] = l->unCheckedNeighbors[i]->data[j];
				}//if
			}//for
		}//if
	}//for


	//////  Jam:
	//////  FIXME:  This is not yet fully done!
}
