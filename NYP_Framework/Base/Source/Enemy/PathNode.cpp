///////////////////////////////////////////////////////////////////////////////
/*!
\file   PathNode.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for PathNode.
*/
///////////////////////////////////////////////////////////////////////////////
#include "PathNode.h"

PathNode::PathNode()
{
	f = g = h = 0;
	parent_i = 0;
	parent_j = 0;
	x = 0;
	y = 0;
}

PathNode::PathNode(int x, int y)
	: f(0)
	, g(0)
	, h(0)
	, parent_i(parent_i)
	, parent_j(parent_j)
	, x(x)
	, y(y)
{
}

PathNode::~PathNode()
{
}
