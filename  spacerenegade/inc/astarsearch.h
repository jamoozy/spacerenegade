#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include "object.h"
#include "ship.h"
#include "factionInfo.h"
#include "vec3.h"
#include <vector>

// Implementation of the A* Search algorithm, used for pathfinding
class AStarSearch
{
private:
	Vec3 startPos; // Current position of the ship
	Vec3 endPos; // Final destination of the ship
	double proximityDistance; // Tolerance for being at a point
	double myRadius; // Radius of the collision sphere of the moving object
	double otherRadius; // Radius of the collision sphere of the other object
	std::vector<Vec3> shortestPath;

public:
	// Constructors/Deconstructors
	AStarSearch(Vec3 start, Vec3 end, double proxDist,
		double myCollisionRadius, double otherCollisionRadius);

	// Methods
	bool pathIsClear(const Vec3& sPos, const Vec3& dPos); /* Determines if there are no
		obstructions between startPos and pos */
	std::vector<Vec3> findShortestPath(); // Computes the shortest path
	Vec3 getFirstWaypoint(); // Finds the first unvisited waypoint to travel to
};

// Structure used for holding intermediate paths while searching
struct Path
{
	std::vector<Vec3> path; // List of the positions visited, in order
	double h; // Estimated cost remaining (Euclidean distance to end)
	double g; // Cost of path so far (sum of distances between points)
	double cost; // h + g

	Path(const std::vector<Vec3>& visitedPositions, const Vec3& end);
	Vec3 getLastPos() { return path.back(); };
};

// Function Object used for Path comparison
class PrioritizePaths
{
public:
	int operator()(const Path &x, const Path &y) // Lower costs have a higher priority
	{ return x.cost > y.cost; };
};

#endif // ASTARSEARCH_H
