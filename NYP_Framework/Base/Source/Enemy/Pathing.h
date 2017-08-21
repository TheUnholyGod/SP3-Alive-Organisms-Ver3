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
	static Coord2D getDelta(Coord2D source_, Coord2D target_);

public:
	static unsigned int manhattan(Coord2D source_, Coord2D target_);
	static unsigned int euclidean(Coord2D source_, Coord2D target_);
	static unsigned int diagonal(Coord2D source_, Coord2D target_);
};

class PathFinder
{
private:
	//Only the PathFinder needs these functions
	bool isOutOfBounds(Coord2D coordinates_);
	bool detectCollision(Coord2D coordinates_);
	Node* findNodeOnList(std::set<Node*>& nodes_, Coord2D coordinates_);
	void DeleteNodeSet(std::set<Node*>& nodes_);

public:
	PathFinder();

	//PathFinding options & configurations
	void setWorldSize(Coord2D worldSize_);
	void setDiagonalMovement(bool enable_);
	/*Heuristic*/
	void setHeuristic(std::function<unsigned int(Coord2D, Coord2D)> heuristic_);

	//Read map from array
	void readMap(int** grid_);

	//Find A-star path
	std::vector<Coord2D> findPath(Coord2D source_, Coord2D target_);

	//Functions for collision in the map
	void addCollision(Coord2D coordinates_);
	void removeCollision(Coord2D coordinates_);
	void clearCollisions();

	//Debug Functions
	void printMap(std::vector<Coord2D> path_, Coord2D src_, Coord2D des_);

private:
	std::function<unsigned int(Coord2D, Coord2D)> heuristic;
	std::vector<Coord2D> direction, walls;
	Coord2D worldSize;
	unsigned int directions;
};