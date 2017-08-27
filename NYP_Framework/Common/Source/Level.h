#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "TileMaker.h"
#include <map>
#include <vector>
#include "Vector3.h"

using std::map;
using std::vector;

class Level
{
	enum DIRECTION
	{
		DOWN = 0,
		LEFT,
		RIGHT,

		TOTAL_DIRETIONS
	};
private:
	int** level_set;
	int m_max_level_size_xy;
	map<int, Tiles*> level_layout;
public:
	Level(int);
	~Level();
	int** GetLevelSetArray();
	void GetLevelLayOut(map<int, Tiles*>&);

	/*
		0 - random room - //Rooms with random blocks 
		1 - room one - //Rooms with only Left or Right exit
		2 - room two - //Rooms with only Left, Right and Bottom exit
		3 - room three - //Rooms with only Left, Right and Top exit
	*/
	void GenerateLevel();
	void GenerateBossLevel();

	int GetSizeOfLevel();  //Returns size of level
	int GetSizeOfTileSet();  //Returns size of one tile
	int GetTotalTilesInLevel();  //Returns the number of tiles in one level

	int ReturnTileViaPos(Vector3, bool);  //Returns the tile via position of the entity/block/etc

	vector<int> ReturnSurroundingTilesViaCurrentTile(int);  //Returns the surrounding tiles in a square of 3 by 3, around the given tile

};

#endif