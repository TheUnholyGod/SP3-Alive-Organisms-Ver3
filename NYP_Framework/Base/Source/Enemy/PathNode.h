///////////////////////////////////////////////////////////////////////////////
/*!
\file   PathNode.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for PathNode. Store the position of the pathNode.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector3.h"

class PathNode
{
public:
	PathNode();
	PathNode(int x, int y);
	~PathNode();

	//Sum of g & h. f = g + h
	float f;
	//Distance between parent & successor
	float g;
	/*Heuristics - Method of finding distance from
	goal to successor node. There are 3 main types:
	Manhattan: UP,DOWN,LEFT,RIGHT.
	Diagonal : UP,DOWN,LEFT,RIGHT & DIAGONAL.
	Euclidean: ANY DIRECTION.
	Note: Specify which movement before pathing.
	*/
	float h;

	//X Position of parent in the map
	int parent_i;
	//Y Position of parent in the map
	int parent_j;
	//X Position of node in the map
	int x;
	//Y Position of node in the map
	int y;

	struct compare 
	{
		bool operator()(const PathNode& a, const PathNode& b)
		{
			return a.f < b.f;
		}
	};
};