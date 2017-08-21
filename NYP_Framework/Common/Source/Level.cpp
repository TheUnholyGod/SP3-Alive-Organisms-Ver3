#include "Level.h"
#include "MyMath.h"

#include <iostream>

Level::Level(int size_of_map_xy)
{
	m_max_level_size_xy = size_of_map_xy;

	level_set = new int*[m_max_level_size_xy];
	for(int i = 0; i < m_max_level_size_xy; ++i)
		level_set[i] = new int[m_max_level_size_xy];


	for (int y = 0; y < m_max_level_size_xy; ++y)
	{
		for (int x = 0; x < m_max_level_size_xy; ++x)
		{
			level_set[y][x] = 0;
		}
	}

	int a;
}

int** Level::GetLevelSetArray()
{
	return level_set;
}

void Level::GetLevelLayOut(map<int, Tiles*>& input_layout)
{
	input_layout = level_layout;
}

void Level::GenerateLevel()
{
	DIRECTION rand_dir = DOWN;
	int x = 0, y = 0;

	x = Math::RandIntMinMax(0, m_max_level_size_xy);
	level_set[y][x] = Math::RandIntMinMax(1, 2);

	while (true)
	{
		rand_dir = static_cast<DIRECTION>(Math::RandIntMinMax(0, TOTAL_DIRETIONS));

		switch (rand_dir)
		{
		case DOWN:
			++y;
			break;
		case LEFT:
			--x;
			if (level_set[y][x] != 0)
				++x;
			break;
		case RIGHT:
			++x;
			if (level_set[y][x] != 0)
				--x;
			break;
		default:
			break;
		}

		if (x < 1 || x >= m_max_level_size_xy - 1)
		{
			if (y >= m_max_level_size_xy - 1)
			{
				level_set[y][x] = 1;

				for (int i = 0; i < m_max_level_size_xy; ++i)
				{
					for (int j = 0; j < m_max_level_size_xy; ++j)
					{
						if (level_set[i][j] == 0)
							level_set[i][j] = ((Math::RandIntMinMax(0, 100) < 50) ? 0 : 3);
					}
				}
				break;
			}
			else
			{
				if (x < 0)
					++x;
				else
					--x;
			}
		}

		if (y >= m_max_level_size_xy)
		{
			--y;
		}

		switch (rand_dir)
		{
		case DOWN:
		{
			if (level_set[y - 1][x] != 2)
			{
				level_set[y - 1][x] = 2;

				level_set[y][x] = 3;
			}
		}
		break;
		case LEFT:
			level_set[y][x] = Math::RandIntMinMax(1, 3);
			break;
		case RIGHT:
			level_set[y][x] = Math::RandIntMinMax(1, 3);
			break;
		default:
			break;
		} 
	}

	int i = 0;
	for (int y = 0; y < m_max_level_size_xy; ++y)
	{
		for (int x = 0; x < m_max_level_size_xy; ++x)
		{
			level_layout[i] = TileMaker::GetInstance()->GenerateRoom(static_cast<TileMaker::TILE_SET_GENERATOR>(level_set[y][x]));
			++i;
		}
	}
}

int Level::GetSizeOfLevel()
{
	return m_max_level_size_xy;
}

int Level::GetSizeOfTileSet()
{
	return MAX_TILE_SIZE_XY;
}

int Level::GetTotalTilesInLevel()
{
	return level_layout.size();
}

int Level::ReturnTileViaPos(Vector3 pos)
{
	Vector3 temp;
	double dividor = 1.0 / MAX_TILE_SIZE_XY;

	temp.x = pos.x * dividor;
	temp.y = pos.y * dividor;

	return ((GetSizeOfLevel() * (((static_cast<int>(temp.y)) < 0) ? 0 : static_cast<int>(temp.y))) + static_cast<int>(temp.x));
}

vector<int> Level::ReturnSurroundingTilesViaCurrentTile(int tile)
{
	vector<int> temp;

	int tile_selected = 0;
	double dividor = 1.0 / (GetSizeOfLevel() - 1);
	int min_max_y = (static_cast<int>(tile * dividor)) * (GetSizeOfLevel() - 1);

	for (int i = -11; i < -8; ++i)
	{
		tile_selected = tile + i;
		if (tile_selected >= 0 && tile_selected < GetTotalTilesInLevel() && tile_selected < min_max_y)
		{
			temp.push_back(tile_selected);
		}
	}

	for (int i = 11; i > 8; --i)
	{
		tile_selected = tile + i;
		if (tile_selected >= 0 && tile_selected < GetTotalTilesInLevel() && tile_selected > min_max_y)
		{
			temp.push_back(tile_selected);
		}
	}

	for (int i = -1; i < 2; ++i)
	{
		tile_selected = tile + i;
		if (tile_selected >= 0 && tile_selected < GetTotalTilesInLevel())
		{
			temp.push_back(tile_selected);
		}
	}

	return temp;
}

