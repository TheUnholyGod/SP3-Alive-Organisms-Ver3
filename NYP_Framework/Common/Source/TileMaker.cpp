#include "TileMaker.h"
#include "MyMath.h"
#include "Loader.h"
#include "BaseDatabase.h"
#include "BaseDatabase.cpp"
#include <iostream>

TileMaker::TileMaker()
{
	tile_set_base = new BaseDatabase<int, Tiles>("tile_set_base", "Assets//LevelGenerators.txt");
	tile_set_base->init();
}

TileMaker::~TileMaker()
{
	this->tile_set_base->Exit();
	delete tile_set_base;
}

/*
	0 - AIR
	1 - SOLID_BLOCK
	2 - PLATFORM
	3 - LADDER
	4 - 20/100 chance of spawning rune,
	5 - 50/100 chance of a platform
	6 - Player Spawner
	7 - Boss Door
	8 - Enemy Node
*/

Tiles* TileMaker::GenerateRoom(TILE_SET_GENERATOR generator)
{
	tiles_set = new Tiles();

	switch (generator)
	{
	case ROOM_RANDOM:
		for (int y = 0; y < MAX_TILE_SIZE_XY - 1; ++y)
		{
			for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
			{
				if(Math::RandIntMinMax(0, 100) < 51)
					tiles_set->SetTilesInArray(x, y, 0);
				else
				{
					if (Math::RandIntMinMax(0, 100) < 51)
						tiles_set->SetTilesInArray(x, y, 2);
					else
						tiles_set->SetTilesInArray(x, y, 3);
				}
			}
		}
		break;
	case ROOM_ONE:
		tiles_set = tile_set_base->getObject(Math::RandIntMinMax(1, 5));
		break;
	case ROOM_TWO:
		tiles_set = tile_set_base->getObject(Math::RandIntMinMax(6, 10));
		break;
	case ROOM_THREE:
		tiles_set = tile_set_base->getObject(Math::RandIntMinMax(11, 15));
		break;
	default:
		break;
	}

	return tiles_set;
}

Tiles::Tiles(string input_file)
{
	m_file_path = input_file;
	level_tiles = new int*[MAX_TILE_SIZE_XY];
	
	for (int i = 0; i < MAX_TILE_SIZE_XY; ++i)
	{
		level_tiles[i] = new int[MAX_TILE_SIZE_XY];
	}

	for (int y = 0; y < MAX_TILE_SIZE_XY; ++y)
	{
		for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
		{
			level_tiles[y][x] = 0;
		}
	}

	Loader::GetInstance()->LoadData(input_file);
	string temp = "";
	std::vector<std::string> data = Loader::GetInstance()->GetData();
	std::vector<std::string>::iterator it = data.begin();

	for (int y = 0; y < MAX_TILE_SIZE_XY; ++y)
	{
		if (it != data.end())
		{
			temp = *it;
		}

		for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
		{
			level_tiles[y][x] = temp[x] - '0';
		}

		++it;
	}

	Loader::GetInstance()->ClearData();

	//for (int y = 0; y < MAX_TILE_SIZE_XY; ++y)
	//{
	//	for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
	//	{
	//		std::cout << level_tiles[y][x];
	//	}
	//	std::cout << std::endl;
	//}
}

Tiles::Tiles()
{
	level_tiles = new int*[MAX_TILE_SIZE_XY];

	for (int i = 0; i < MAX_TILE_SIZE_XY; ++i)
	{
		level_tiles[i] = new int[MAX_TILE_SIZE_XY];
	}

	for (int y = 0; y < MAX_TILE_SIZE_XY; ++y)
	{
		for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
		{
			level_tiles[y][x] = 0;
		}
	}
}

int** Tiles::GetTilesArray()
{
	return level_tiles;
}

void Tiles::SetTilesInArray(int x, int y, int input)
{
	level_tiles[y][x] = input;
}

Tiles& Tiles::operator=(Tiles* input)
{
	for (int y = 0; y < MAX_TILE_SIZE_XY; ++y)
	{
		for (int x = 0; x < MAX_TILE_SIZE_XY; ++x)
		{
			this->level_tiles[y][x] = input->level_tiles[y][x];
		}
	}

	return *this;
}