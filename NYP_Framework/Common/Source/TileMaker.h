#include <string>
#include "SingletonTemplate.h"

#define MAX_TILE_SIZE_XY 7

using std::string;

#ifndef _TILES_H_
#define _TILES_H_

class Tiles
{
private:
	string m_file_path;
	int** level_tiles;

public:
	Tiles(string);
	Tiles();

	int** GetTilesArray();
	void SetTilesInArray(int, int, int);

	Tiles& operator=(Tiles*);

};

#endif

#ifndef _TILE_MAKER_H_
#define _TILE_MAKER_H_

template<class Key,class T>
class BaseDatabase;

class TileMaker : public Singleton<TileMaker>
{
	friend Singleton <TileMaker>;

private:
	Tiles* tiles_set;
	BaseDatabase<int, Tiles>* tile_set_base;
	TileMaker();
	~TileMaker();

public:
	
	enum TILE_SET_GENERATOR
	{
		ROOM_RANDOM = 0,
		ROOM_ONE,
		ROOM_TWO,
		ROOM_THREE,

		TOTAL_ROOMS
	};

	Tiles* GenerateRoom(TILE_SET_GENERATOR);  //ID: 0 - Random, 1 - Room One, 2 - Room Two, 3 - Room 3
};

#endif