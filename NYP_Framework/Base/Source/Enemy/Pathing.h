///////////////////////////////////////////////////////////////////////////////
/*!
\file   Pathing.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for Pathing. Store and updates previous and next pathNodes. AKA the
Navigator of the AI.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <functional>
#include <set>
#include <algorithm>
#include <iostream>

struct Coord2D
{
	Coord2D() {};
	Coord2D(int x, int y) : x(x), y(y) {};
	int x, y;

	//Used to compare Coord2D
	bool operator == (const Coord2D& coordinates_);
	Coord2D operator - (const Coord2D& coordinates_);
};

struct Node
{
	unsigned int G, H; //Used to calculate the 'Score'. A.K.A Determine the shortest path
	Coord2D coordinates; //Coordinate of this node
	Node *parent; //Used to trace the path

	Node(Coord2D coord_, Node *parent_ = nullptr);
	unsigned int getScore();
};

class Heuristic
{
	//Function based on method to find score/priority
	static Coord2D getDelta(Coord2D source_, Coord2D target_);

public:
	static unsigned int manhattan(Coord2D source_, Coord2D target_);
	static unsigned int euclidean(Coord2D source_, Coord2D target_);
	static unsigned int diagonal(Coord2D source_, Coord2D target_);
};

class PathFinder
{
private:
	

public:
	PathFinder();
	~PathFinder();

	//Some helper functions
	bool isOutOfBounds(Coord2D coordinates_);
	bool detectCollision(Coord2D coordinates_);
	Node* findNodeOnList(std::set<Node*>& nodes_, Coord2D coordinates_);
	void DeleteNodeSet(std::set<Node*>& nodes_);
	
	//PathFinding options & configurations
	void setWorldSize(Coord2D worldSize_);
	void setDiagonalMovement(bool enable_);
	/*
	Heuristics(Use according to movement for optimised search speed):
	Manhattan - 4 Direction movement
	Euclidean - Any direction
	Diagonal  - 8 Direction movement
	*/
	void setHeuristic(std::function<unsigned int(Coord2D, Coord2D)> heuristic_);

	//Read map from array & set collisions
	void readMap(int** grid_);

	/*Find A-star path
	Returns vector of Coord2D
	Destination at index[0]*/
	std::vector<Coord2D> findPath(Coord2D source_, Coord2D target_);

	//Add collision in the form of 2D coordinate
	void addCollision(Coord2D coordinates_);
	//Remove collision in the form of 2D coordinate
	void removeCollision(Coord2D coordinates_);
	//Clear all the collision in the map
	void clearCollisions();

	//Debug Function - Print Map with routes marked
	void printMap(std::vector<Coord2D> path_, Coord2D src_, Coord2D des_);

private:
	std::function<unsigned int(Coord2D, Coord2D)> heuristic; //Method of calculating "Score" or Priority of node. Determines if its closer or further
	std::vector<Coord2D> direction, walls; //Storage for walls and direction
	Coord2D worldSize; //Size of the world
	unsigned int directions; //Num of directions the player can move in
};