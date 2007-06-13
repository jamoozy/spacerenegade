#include "object.h"
#include "ship.h"
#include "factionInfo.h"
#include "vec3.h"
#include "astarsearch.h"
#include "environment.h"
#include <vector>
#include <queue>
#include <iostream>

using namespace std;
extern OctTree *env;

// Helper function for finding a Vec3 in a vector of Vec3's
bool isInVector(const vector<Vec3>& v, const Vec3& pos)
{
	for(unsigned int i = 0; i < v.size(); i++ )
	{
	    if(v.at(i) == pos)
	    	return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Path Object ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

// Constructors/Deconstructors
Path::Path(const vector<Vec3>& visitedPositions, const Vec3& end) :
		path(visitedPositions), h(0.0), g(0.0), cost(0.0)
{
	// Heuristic: Euclidean distance from last visited point to end
	h = path.back().distance(end);

	// Sum up the distances traveled
	for(unsigned int i = 0; i < path.size() - 1; i++)
	{
		g += path.at(i).distance(path.at(i + 1));
	}

	// Compute total cost
	cost = h + g;
}

ostream& operator << (ostream& os, Path& p)
{
	os << "Path: h: " << p.h << ", g: " << p.g << ", cost: " << p.cost << endl;
	os << "    ";
	for(unsigned int i = 0; i < p.path.size(); i++)
	{
		Vec3 point = p.path.at(i);
		os << "(" << point.x() << ", " << point.y() << ", " << point.z() << ") ";
	}
	os << endl;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
// -------------------------- AStarSearch Object ---------------------------- //
////////////////////////////////////////////////////////////////////////////////
// Constructors/Deconstructors
AStarSearch::AStarSearch(Vec3 start, Vec3 end, double proxDist,
		double myCollisionRadius, double otherCollisionRadius) : startPos(start), 
		endPos(end), proximityDistance(proxDist), myRadius(myCollisionRadius),
		otherRadius(otherCollisionRadius)
{}

// Helper Methods
bool AStarSearch::pathIsClear(const Vec3& sPos, const Vec3& dPos)
{
	double distance = sPos.distance(dPos);
	// Don't check for collisions at endPos, as there will be one
	if(dPos == endPos)
		distance = distance - myRadius - otherRadius - proximityDistance;
	Vec3 direction = dPos - sPos;
	direction.normalize();

	// Create a temporary dummy object to test for collisions
	Object dummy("", myRadius);

	/* Place an object with a collision sphere radius of x does not collide at
		each interval along the path from sPos to dPos */
	double interval = myRadius + 1;
	while(interval != distance)
	{
		// Make sure that the interval does not overshoot the distance
		if(interval > distance)
			interval = distance;
		else
			interval += myRadius;

		// Calculate new interval along the path
		Vec3 pathInterval = direction * interval;
		pathInterval = pathInterval + sPos;

		// Set new positon and do collision test
		dummy.setAt(pathInterval);
		env->add(&dummy);
		Leaf* residence = dummy.getResidence();
		// Check all the objects in the local node
		for (unsigned int i = 0; i < residence->data.size(); i++)
		{
			Object* otherObject = residence->data.at(i);
			if (&dummy != otherObject && !otherObject->aiShouldConsider() 
					&& dummy.collidesWith(otherObject))
				return false;
		}

		// Check all the objects in the neighboring nodes.
		for (int j = 0; j < 13; j++)
		{
			if (residence->checkedNeighbors[j] != NULL)
			{
				for (unsigned int k = 0; k < residence->checkedNeighbors[j]->data.size(); k++)
				{
					Object* otherObject = residence->checkedNeighbors[j]->data.at(k);
					if (&dummy != otherObject && !otherObject->aiShouldConsider()
							&& dummy.collidesWith(otherObject))
						return false;
				}
			}
			// Repeat for the Unchecked neighbors
			if (residence->unCheckedNeighbors[j] != NULL)
			{
				for (unsigned int k = 0; k < residence->unCheckedNeighbors[j]->data.size(); k++)
				{
					Object* otherObject = residence->unCheckedNeighbors[j]->data.at(k);
					if (&dummy != otherObject && !otherObject->aiShouldConsider()
							&& dummy.collidesWith(otherObject))
						return false;
				}
			}
		}

		residence->remove(&dummy);
	}

	// Path is clear
	return true;
}

// Methods
vector<Vec3> AStarSearch::findShortestPath()
{
	// For more information about A* search, including the pseudocode for this
	// algorithm, see http://en.wikipedia.org/wiki/A%2A_search

	// If the ship is there, do not search
	if(endPos.distance(startPos) <= (myRadius + otherRadius + proximityDistance + 2))
	{
		vector<Vec3> path;
		path.push_back(startPos);
		path.push_back(endPos);
		return path;
	}
	// Create a priority queue of paths, using PrioritizePaths for comparison
	priority_queue<Path, vector<Path>, PrioritizePaths> pq;

	// Initialize the PQ with an empty path
	vector<Vec3> startPath;
	vector<Vec3> closed;
	startPath.push_back(startPos);
	pq.push(Path(startPath, endPos));
	//cout << endl << "New search: " << endl;

	// As long as pq is not empty, a possible path exists
	while(!pq.empty())
	{
		Path p = pq.top();
		//cout << p;
		pq.pop();
		Vec3 x = p.getLastPos();
		vector<Vec3> curPath = p.path;
		// x has been visited
		if(isInVector(closed, x))
			continue;
		// The shortest path has been found
		else if(x == endPos)
		{
			// Save the path
			vector<Vec3>::iterator iter;
			for( iter = p.path.begin(); iter != p.path.end(); iter++ )
			{
				shortestPath.push_back(*iter);
			}
			return shortestPath;
		}
		// Mark x as visited
		closed.push_back(x);

		// Enqueue the possible paths

		// Check to see if a direct path is clear
		if(pathIsClear(x, endPos))
		{
			vector<Vec3> pathToEnd = curPath;
			pathToEnd.push_back(endPos);
			pq.push(Path(pathToEnd, endPos));
		}

		// Find the current leaf node for x
		Object anchor("");
		anchor.setAt(x);
		env->add(&anchor);
		Leaf* residence = anchor.getResidence();
		// Enqueue local node if it is not x
		Vec3 center = residence->center();
		double distance = x.distance(center);
		if((distance > proximityDistance) && pathIsClear(x, center))
		{
			vector<Vec3> pathToCenter = curPath;
			pathToCenter.push_back(center);
			Path newPath = Path(pathToCenter, endPos);
			pq.push(newPath);
			//cout << "         *Costs to (" << center.x() << ", " << center.y() << ", " << center.z() << ") -  h:" 
			//	<< newPath.h << " g: " << newPath.g << " cost: " << newPath.cost << endl;
		}
		// Enqueue Neighboring Nodes
		for(unsigned int i = 0; i < 13; i++)
		{

			if (residence->checkedNeighbors[i] != NULL && pathIsClear(x, residence->checkedNeighbors[i]->center()))
			{
				Vec3 nodeCenter = residence->checkedNeighbors[i]->center();
//				double d = x.distance(nodeCenter);
				vector<Vec3> pathToNode = curPath;
				pathToNode.push_back(nodeCenter);
				Path newPath = Path(pathToNode, endPos);
				pq.push(newPath);
				//cout << "       " << i << "c*Costs to (" << nodeCenter.x() << ", " << nodeCenter.y() << ", " << nodeCenter.z() << ") -  h:" 
				//	<< newPath.h << " g: " << newPath.g << " cost: " << newPath.cost << endl;
			}
			if (residence->unCheckedNeighbors[i] != NULL && pathIsClear(x, residence->unCheckedNeighbors[i]->center()))
			{
				Vec3 nodeCenter = residence->unCheckedNeighbors[i]->center();
//				double d = x.distance(nodeCenter);
				vector<Vec3> pathToNode = curPath;
				pathToNode.push_back(residence->unCheckedNeighbors[i]->center());
				Path newPath = Path(pathToNode, endPos);
				pq.push(newPath);
				//cout << "       " << i << "u*Costs to (" << nodeCenter.x() << ", " << nodeCenter.y() << ", " << nodeCenter.z() << ") -  h:" 
				//	<< newPath.h << " g: " << newPath.g << " cost: " << newPath.cost << endl;		
			}
		}
		residence->remove(&anchor);
	}
	// No Path is Found
/*	if(shortestPath.size() < 2)
	{
		shortestPath.push_back(startPos);
		shortestPath.push_back(endPos);
	}
*/
	return shortestPath;
}

Vec3 AStarSearch::getFirstWaypoint()
{
	/* Note: There should always be atleast two waypoints: startPos and endPos.
		Therefore, the second slot will always be the first unvisited waypoint */
	return shortestPath.at(1);
}
