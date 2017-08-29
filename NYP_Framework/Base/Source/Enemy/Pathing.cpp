///////////////////////////////////////////////////////////////////////////////
/*!
\file   Pathing.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for Pathing.
*/
///////////////////////////////////////////////////////////////////////////////
#include "Pathing.h"

#include "../PlayerInfo/PlayerInfo.h"
#include "../MapManager.h"


using namespace std::placeholders;

bool Coord2D::operator == (const Coord2D& coordinates_)
{
	return (x == coordinates_.x && y == coordinates_.y);
}

Coord2D Coord2D::operator-(const Coord2D & coordinates_)
{
	return (Coord2D(this->x - coordinates_.x, this->y - coordinates_.y));
}

Coord2D operator + (const Coord2D& left_, const Coord2D& right_)
{
	return{ left_.x + right_.x, left_.y + right_.y };
}

Node::Node(Coord2D coordinates_, Node *parent_)
{
	parent = parent_;
	coordinates = coordinates_;
	G = H = 0;
}

unsigned int Node::getScore()
{
	return G + H;
}

PathFinder::PathFinder()
{
	setDiagonalMovement(false);
	setHeuristic(&Heuristic::manhattan);
	direction = {
		{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 0 }, //UP,RIGHT,DOWN,LEFT
		{ -1, -1 },{ 1, 1 },{ -1, 1 },{ 1, -1 }//SW,NE,NW,EW
	};
	//worldSize.x = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->GetSizeOfLevel() * 7;
	//worldSize.y = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->GetSizeOfLevel() * 7;

	worldSize = { 224,224 };
}

PathFinder::~PathFinder()
{
	if(heuristic)
		heuristic.~function();
	direction.clear();
	walls.clear();
}

void PathFinder::setWorldSize(Coord2D worldSize_)
{
	worldSize = worldSize_;
}

void PathFinder::setDiagonalMovement(bool enable_)
{
	directions = (enable_ ? 8 : 4);
}

void PathFinder::setHeuristic(std::function<unsigned int(Coord2D, Coord2D)> heuristic_)
{
	heuristic = std::bind(heuristic_, _1, _2);
}

void PathFinder::readMap(int ** grid_)
{
	for (int x = 0; x < worldSize.x; ++x)
	{
		for (int y = 0; y < worldSize.y; ++y)
		{
			//Check for air blocks, if theres air under it, its not passable.
			if (grid_[y][x] == 0)
			{
				if (y - 1 < 0)
					continue;

				if (grid_[y - 1][x] == 0)
					addCollision({ x,y });

				continue;
			}
			if (grid_[y][x] == 3 ||
				grid_[y][x] == 4 ||
				grid_[y][x] == 6 ||
				grid_[y][x] == 7 ||
				grid_[y][x] == 8) //Passable blocks
			{
				continue;
			}
			else //Collidable
			{
				addCollision({ x,y });
			}
		}
	}
}

void PathFinder::addCollision(Coord2D coordinates_)
{
	walls.push_back(coordinates_);
}

void PathFinder::removeCollision(Coord2D coordinates_)
{
	auto it = std::find(walls.begin(), walls.end(), coordinates_);
	if (it != walls.end())
	{
		walls.erase(it);
	}
}

void PathFinder::clearCollisions()
{
	walls.clear();
}

void PathFinder::printMap(std::vector<Coord2D> path_, Coord2D src_, Coord2D des_)
{
	for (int y = worldSize.y - 1; y >= 0; --y)
	{
		for (int x = 0; x < worldSize.x; ++x)
		{
			bool path = false;
			//Check for path
			for (int i = 0; i < path_.size(); i++)
			{
				if (path_[i].x == x && path_[i].y == y)
				{
					std::cout << "*";
					path = true;
					break;
				}
			}
			if (!path)
			{
				if (detectCollision({ x,y })) //Collidable blocks
					std::cout << "1";
				else
					std::cout << "0";
			}
		}
		std::cout << std::endl;
	}
}

std::vector<Coord2D> PathFinder::findPath(Coord2D source_, Coord2D target_)
{
	//Check the source and target before proceeding to calculations
	//to save some processing time lol

	std::vector<Coord2D> path; //Vector of Coord2D to return

	////If the source and target are out of bounds
	//if (isOutOfBounds(target_)) { std::cout << "Target out of range" << std::endl; return path; }
	//if (isOutOfBounds(source_)) { std::cout << "Source out of range" << std::endl; return path; }
	////If the source or target are blocked
	//if (detectCollision(target_)) { std::cout << "Target is blocked" << std::endl; return path; }
	//if (detectCollision(source_)) { std::cout << "Source is blocked" << std::endl; return path; }

	Node *current = nullptr;
	std::set<Node*> openSet, closedSet;
	openSet.insert(new Node(source_));

	//Boolean to determine whether the path was completed
	bool isPathFound = false;

	//Loop to find all successor nodes and check validity
	while (!openSet.empty())
	{
		current = *openSet.begin();
		//Find the lowest score node (Lower score means it's closer to destination)
		for (auto node : openSet)
		{
			if (node->getScore() <= current->getScore())
				current = node;
		}

		//Found destination, break out of loop
		if (current->coordinates == target_)
		{
			isPathFound = true;
			break;
		}

		//Not destination, mark it inside closedSet
		//Remove it from openSet
		closedSet.insert(current);
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));

		//Check the surrounding nodes
		for (unsigned int i = 0; i < directions; ++i)
		{
			Coord2D newCoordinates(current->coordinates + direction[i]);

			/*
			Check whether Coordinate is eligible to be checked
			If collision detected, continue the loop
			If coordinate is in closedSet, continue the loop
			If coordinate lies outside of the map, continue the loop
			*/
			if (detectCollision(newCoordinates) ||
				findNodeOnList(closedSet, newCoordinates) ||
				isOutOfBounds(newCoordinates))
			{
				continue;
			}

			unsigned int totalCost = current->G + ((i < 4) ? 10 : 14);

			Node *successor = findNodeOnList(openSet, newCoordinates);
			if (successor == nullptr)
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target_);
				openSet.insert(successor);
			}
			else if (totalCost < successor->G)
			{
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	//If path is found, return the vector of coords
	if (isPathFound)
	{
		while (current != nullptr)
		{
			path.push_back(current->coordinates);
			current = current->parent;
		}
		std::cout << "Path found!" << std::endl;
		DeleteNodeSet(openSet);
		DeleteNodeSet(closedSet);

		//Reverse the results: 0 is start, Size() is destination
		std::reverse(path.begin(), path.end());
		/*Debug printing of routemap*/
		//printMap(path, source_, target_);

		return path;
	}
	else //No path found, return empty vector
	{
		std::cout << "Failed to find the full path." << std::endl;
		DeleteNodeSet(openSet);
		DeleteNodeSet(closedSet);
		//delete current;
		return path;
	}
}

Node* PathFinder::findNodeOnList(std::set<Node*>& nodes_, Coord2D coordinates_)
{
	for (auto node : nodes_)
	{
		if (node->coordinates == coordinates_)
		{
			return node;
		}
	}
	return nullptr;
}

void PathFinder::DeleteNodeSet(std::set<Node*>& nodes_)
{
	for (auto it = nodes_.begin(); it != nodes_.end();)
	{
		delete *it;
		it = nodes_.erase(it);
	}
}

bool PathFinder::isOutOfBounds(Coord2D coordinates_)
{
	if (coordinates_.x > worldSize.x - 1 || coordinates_.x < 0 ||
		coordinates_.y > worldSize.y - 1 || coordinates_.y < 0)
		return true;
	else
		return false;
}

bool PathFinder::detectCollision(Coord2D coordinates_)
{
	if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
		coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
	{
		return true;
	}
	return false;
}

Coord2D Heuristic::getDelta(Coord2D source_, Coord2D target_)
{
	return{ abs(source_.x - target_.x),  abs(source_.y - target_.y) };
}

unsigned int Heuristic::manhattan(Coord2D source_, Coord2D target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<unsigned int>(10 * (delta.x + delta.y));
}

unsigned int Heuristic::euclidean(Coord2D source_, Coord2D target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<unsigned int>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

unsigned int Heuristic::diagonal(Coord2D source_, Coord2D target_)
{
	auto delta = std::move(getDelta(source_, target_));
	return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}