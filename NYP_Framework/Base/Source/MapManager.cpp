#include "MapManager.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"

#include "TileEntitySolidBlock.h"

void MapManager::Init()
{
	for (int i = 0; i < 5; ++i)
	{
		Level *level = new Level(10);
		level->GenerateLevel();
		
		map_database[i] = level;
	}
}

/*
	0 - AIR
	1 - SOLID_BLOCK
	2 - PLATFORM
	3 - LADDER
	4 - 20/100 chance of spawning rune,
	5 - 50/100 chance of a platform
	6 - Player Spawner
*/
void MapManager::GenerateBlocks(int level)
{
	map<int, Tiles*> temp;
	Vector3 temp_player_pos(0, 0, 0);
	map_database[level]->GetLevelLayOut(temp);
	int section = 0, row = 0;
	bool set_player_pos = false;
	for (int i = 0; i < temp.size(); ++i)
	{

		for (int y = 0; y < map_database[level]->GetSizeOfTileSet(); ++y)
		{
			for (int x = 0; x < map_database[level]->GetSizeOfTileSet(); ++x)
			{

				switch (temp[i]->GetTilesArray()[y][x])
				{
				case 1:
					Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(1, 1, 1), true, true, true, i);
					break;
				case 2:
					Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)), (y + (section * 7)) + 0.35, 0), Vector3(1, 0.3, 1), true, true, true, i);
					break;
				case 3:
				{
					if(y + 1 < map_database[level]->GetSizeOfTileSet() && temp[i]->GetTilesArray()[y + 1][x] != 3)
						Create::TileEntityCreator(TileEntity::LADDERWITHPLATFORM, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(1, 1, 1), true, true, true, i);
					else
						Create::TileEntityCreator(TileEntity::LADDER, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(1, 1, 1), true, true, true, i);
				}
					break;
				case 4:
				{
					if (Math::RandIntMinMax(0, 100) < 20)
						Create::TileEntityCreator(TileEntity::RUNE_SPAWNER, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(0.5, 1, 1), true, true, true, i);
				}
					break;
				case 5:
				{
					if (Math::RandIntMinMax(0, 100) < 50)
						Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)), (y + (section * 7)) + 0.35, 0), Vector3(1, 0.3, 1), true, true, true, i);
				}
				break;
				case 6:
				{
					if (!set_player_pos && i >= (temp.size() - 10))
					{
						Player::GetInstance()->SetPosition(Vector3((x + (row * 7)), (y + (section * 7)) - 0.2, 0));
						set_player_pos = true;
					}

					temp_player_pos.Set((x + (row * 7)), (y + (section * 7)) - 0.2, 0);
				}
				break;
				default:
					break;
				}
			}
		}

		++row;

		if ((i + 1) % map_database[level]->GetSizeOfLevel() == 0)
		{
			row = 0;
			++section;
		}
	}

	if (!set_player_pos)
	{
		Player::GetInstance()->SetPosition(temp_player_pos);
	}

	for (int y = 0; y < map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet(); ++y)
	{
		for (int x = 0; x < map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet(); ++x)
		{
			if (y == 0 || y == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
			{
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x, y + ((y == 0) ? -1 : 1), 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0)));
				if (x == 0 || x == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
					Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x + ((x == 0) ? -1 : 1), y, 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0)));

			}
			else if (x == 0 || x == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x + ((x == 0) ? -1 : 1), y, 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0)));

		}
	}

	GenerateMapArray(level);
}

void MapManager::DeleteAllLevels()
{
	for (int i = 0; i < map_database.size(); ++i)
	{
		delete map_database[i];
		map_database[i] = nullptr;
	}
}

int ** MapManager::getMapArray()
{
	return m_map_array;
}

void MapManager::GenerateMapArray(int level)
{
	map<int, Tiles*> temp;
	map_database[level]->GetLevelLayOut(temp);

	//Initialise new array
	Level* l = GetLevel(level);
	int size = l->GetSizeOfLevel() * 7;

	m_map_array = new int*[size];
	for (int h = 0; h < size; ++h)
	{
		m_map_array[h] = new int[size];
		for (int w = 0; w < size; ++w)
		{
			m_map_array[h][w] = 0;
		}
	}

	int section = 0, row = 0;
	for (int i = 0; i < temp.size(); ++i)
	{
		for (int y = 0; y < map_database[level]->GetSizeOfTileSet(); ++y)
		{
			for (int x = 0; x < map_database[level]->GetSizeOfTileSet(); ++x)
			{
				if (temp[i]->GetTilesArray()[y][x] == 0 || temp[i]->GetTilesArray()[y][x] == 6)
				{
					m_map_array[y + (section * 7)][x + (row * 7)] = 0;
				}
				else
					m_map_array[y + (section * 7)][x + (row * 7)] = temp[i]->GetTilesArray()[y][x];
			}
		}

		++row;

		if ((i + 1) % map_database[level]->GetSizeOfLevel() == 0)
		{
			row = 0;
			++section;
		}
	}

	////Initialise new array
	//Level* l = GetLevel(level);
	//int size = l->GetSizeOfLevel() * 7;

	//new_map_array = new int*[size];
	//for (int h = 0; h < size; ++h)
	//{
	//	new_map_array[h] = new int[size];
	//	for (int w = 0; w < size; ++w)
	//	{
	//		new_map_array[h][w] = 0;
	//	}
	//}

	//Change the map before printing
	// y,x
	//m_map_array[1][0] = 3;

	//Correct map to allow easier debug
	/*for (int x = 10 * 7 - 1, x2 = 0; x >= 0 && x2 < 10 * 7; --x, ++x2)
	{
		for (int y = 0; y < 10 * 7; ++y)
		{
			new_map_array[x2][y] = m_map_array[x][y];
		}
	}*/
	
	//Printing for debug
	/*for (int y = size - 1; y >= 0; --y)
	{
		for (int x = 0; x < size; ++x)
		{
			std::cout << m_map_array[y][x];
		}
		std::cout << std::endl;
	}*/
}

Level* MapManager::GetLevel(int level)
{
	return map_database[level];
}

TileEntity* Create::TileEntityCreator(const TileEntity::BLOCK_TYPE block_type,
	const Vector3& _position,
	const Vector3& _scale,
	bool have_collider,
	bool have_physic,
	bool is_static,
	int current_tile_ID)
{
	switch (block_type)
	{

	case TileEntity::SOLID_BLOCK:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("solid_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case TileEntity::LADDER:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("ladder_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case TileEntity::LADDERWITHPLATFORM:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("platform_ladder_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case TileEntity::TOP_PLATFORM:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("platform_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case TileEntity::RUNE_SPAWNER:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("rune_spawner_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	default:
		return nullptr;
	}
}
