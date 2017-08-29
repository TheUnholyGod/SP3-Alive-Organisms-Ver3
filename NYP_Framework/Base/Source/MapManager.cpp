#include "MapManager.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "PlayerInfo\PlayerInfo.h"

#include "TileEntitySolidBlock.h"
#include "Enemy\EnemyBase.h"
#include "EntityDrop.h"

void MapManager::Init()
{
	for (int i = 0; i < 5; ++i)
	{
		Level *level = new Level(3 * (i + 1));
		level->GenerateLevel();
		
		map_database[i] = level;

		level = new Level(2);
		level->GenerateBossLevel();

		boss_map_database[i] = level;
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
	7 - Boss Door
	8 - Enemy Node
*/
void MapManager::GenerateBlocks(int level)
{
	map<int, Tiles*> temp;
	map<int, vector<Vector3>> enemy_temp_list, flying_temp_list;
	Vector3 temp_player_pos(0, 0, 0), temp_door_pos(0, 0, 0);
	map_database[level]->GetLevelLayOut(temp);
	int section = 0, row = 0;
	bool set_player_pos = false, set_boss_door = false;
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
				{
					Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)), (y + (section * 7)) + 0.35, 0), Vector3(1, 0.3, 1), true, true, true, i);
					if (i < (temp.size() - MapManager::GetInstance()->GetLevel(level)->GetSizeOfLevel()) && !set_boss_door)
					{
						temp_door_pos = Vector3((x + (row * 7)), (y + (section * 7)) + 1, 0.14);
					}

					if (i < (temp.size() - MapManager::GetInstance()->GetLevel(level)->GetSizeOfLevel()) && !set_player_pos)
					{
						temp_player_pos = Vector3((x + (row * 7)), (y + (section * 7)) + 1.1, 0);
					}
				}
					break;
				case 3:
				{
					if (y + 1 < map_database[level]->GetSizeOfTileSet())
					{
						if (temp[i]->GetTilesArray()[y + 1][x] != 3)
							Create::TileEntityCreator(TileEntity::LADDERWITHPLATFORM, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(1, 1, 1), true, true, true, i);
						else
							Create::TileEntityCreator(TileEntity::LADDER, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(0.3, 1, 1), true, true, true, i);
					}
					else
						Create::TileEntityCreator(TileEntity::LADDERWITHPLATFORM, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(1, 1, 1), true, true, true, i);
				}
					break;
				case 4:
				{
					if (Math::RandIntMinMax(0, 100) < 20)
					{
						Create::Rune(Vector3((x + (row * 7)) + 0.01, y + (section * 7) + 0.15, 0.01), Vector3(0.35, 0.35, 0.35), i);
						Create::TileEntityCreator(TileEntity::RUNE_SPAWNER, Vector3((x + (row * 7)), y + (section * 7), 0), Vector3(0.5, 1, 1), true, true, true, i);
					}
				}
					break;
				case 5:
				{
					if (Math::RandIntMinMax(0, 100) < 50)
					{
						Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)), (y + (section * 7)) + 0.35, 0), Vector3(1, 0.3, 1), true, true, true, i);
					}
					else
						temp[i]->SetTilesInArray(x, y, 0);
				}
				break;
				case 6:
				{
					if (!set_player_pos && i >= (temp.size() - 10))
					{
						player_starting_pos.push_back(Vector3((x + (row * 7)), (y + (section * 7)) - 0.2, 0));
						Create::TileEntityCreator(TileEntity::DOOR_EXIT, Vector3((x + (row * 7)), (y + (section * 7)) + 0.2, 0.14), Vector3(1, 1.5, 1), true, true, true, i);

						set_player_pos = true;
					}

					temp_player_pos.Set((x + (row * 7)), (y + (section * 7)) - 0.2, 0);
				}
				break;
				case 7:
				{
					if (!set_boss_door)
					{
						if (Math::RandIntMinMax(0, 200) < 50 && i < (temp.size() - 10))
						{
							std::cout << "X: " << (x + (row * 7)) << " Y: " << (y + (section * 7)) << std::endl;
							set_boss_door = true;
							Create::TileEntityCreator(TileEntity::BOSS_DOOR, Vector3((x + (row * 7)), (y + (section * 7)) + 0.2, 0.14), Vector3(1, 1.5, 1), true, true, true, i);
							Create::Entity("reference", Vector3((x + (row * 7)), (y + (section * 7)), 0)); // Reference
						}
					}
					if (temp_player_pos.x == 0)
						temp_door_pos.Set((x + (row * 7)), (y + (section * 7)) + 0.35, 0);
				}
				break;
				case 8:
				{
					if (Math::RandIntMinMax(0, 100) < 20)
						enemy_temp_list[i].push_back(Vector3((x + (row * 7)), y + (section * 7), 0));
					else
					{
						if (Math::RandIntMinMax(0, 100) < 60)
							flying_temp_list[i].push_back(Vector3((x + (row * 7)), y + (section * 7), 0));
					}
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
		player_starting_pos.push_back(temp_player_pos);
		Create::TileEntityCreator(TileEntity::DOOR_EXIT, Vector3(temp_player_pos.x, temp_player_pos.y + 0.2, 0.14), Vector3(1, 1.5, 1), true, true, true, MapManager::GetInstance()->GetLevel(level)->ReturnTileViaPos(Vector3(temp_player_pos.x, temp_player_pos.y + 0.2, 0.14), false));
	}

	if (!set_boss_door)
	{
		std::cout << "X: " << temp_door_pos.x << " Y: " << temp_door_pos.y << std::endl;
		Create::TileEntityCreator(TileEntity::BOSS_DOOR, Vector3(temp_door_pos.x, temp_door_pos.y + 0.2, 0.14), Vector3(1, 1.5, 1), true, true, false, GetLevel(level)->ReturnTileViaPos(temp_door_pos, Player::GetInstance()->GetIsFightingBoss()));
		Create::Entity("reference", temp_door_pos); // Reference
	}

	for (int y = 0; y < map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet(); ++y)
	{
		for (int x = 0; x < map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet(); ++x)
		{
			if (y == 0 || y == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
			{
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x, y + ((y == 0) ? -1 : 1), 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()));
				if (x == 0 || x == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
					Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x + ((x == 0) ? -1 : 1), y, 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()));

			}
			else if (x == 0 || x == (map_database[level]->GetSizeOfLevel() * map_database[level]->GetSizeOfTileSet()) - 1)
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x + ((x == 0) ? -1 : 1), y, 0), Vector3(1, 1, 1), true, true, true, map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()));

		}
	}

	GenerateMapArray(level);


	if (enemy_temp_list.size() > 0)
	{
		for (map<int, vector<Vector3>>::iterator it = enemy_temp_list.begin(); it != enemy_temp_list.end(); ++it)
		{
			for (vector<Vector3>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				Create::Enemy(EnemyBase::ENEMY_TYPE::E_MELEE, Vector3(it2->x, it2->y, it2->z + 0.01), Vector3(1, 1, 1), true, false, false, it->first);
			}
		}
	}

	if (flying_temp_list.size() > 0)
	{
		for (map<int, vector<Vector3>>::iterator it = flying_temp_list.begin(); it != flying_temp_list.end(); ++it)
		{
			for (vector<Vector3>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				Create::Enemy(EnemyBase::ENEMY_TYPE::E_FLYING, Vector3(it2->x, it2->y, it2->z + 0.01), Vector3(1, 1, 1), true, false, false, it->first);
			}
		}
	}
}

void MapManager::GenerateBossBlocks(int level)
{
	map<int, Tiles*> temp;
	map<int, vector<Vector3>> enemy_temp_list;
	Vector3 temp_player_pos(0, 0, 0), temp_door_pos(0, 0, 0);
	boss_map_database[level]->GetLevelLayOut(temp);
	int section = 0, row = 0;
	for (int i = 0; i < temp.size(); ++i)
	{

		for (int y = 0; y < boss_map_database[level]->GetSizeOfTileSet(); ++y)
		{
			for (int x = 0; x < boss_map_database[level]->GetSizeOfTileSet(); ++x)
			{
				switch (temp[i]->GetTilesArray()[y][x])
				{
				case 1:
					Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3((x + (row * 7)) + 100, y + (section * 7) + 100, 0), Vector3(1, 1, 1), true, true, true, i, true);
					break;
				case 2:
					Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)) + 100, ((y + (section * 7)) + 0.35) + 100, 0), Vector3(1, 0.3, 1), true, true, true, i, true);
					break;
				case 3:
				{
					if (y + 1 < boss_map_database[level]->GetSizeOfTileSet())
					{
						if (temp[i]->GetTilesArray()[y + 1][x] != 3)
							Create::TileEntityCreator(TileEntity::LADDERWITHPLATFORM, Vector3((x + (row * 7)) + 100, y + (section * 7) + 100, 0), Vector3(1, 1, 1), true, true, true, i, true);
						else
							Create::TileEntityCreator(TileEntity::LADDER, Vector3((x + (row * 7)) + 100, y + (section * 7) + 100, 0), Vector3(0.3, 1, 1), true, true, true, i, true);
					}
					else
						Create::TileEntityCreator(TileEntity::LADDERWITHPLATFORM, Vector3((x + (row * 7)) + 100, y + (section * 7) + 100, 0), Vector3(1, 1, 1), true, true, true, i, true);
				}
				break;
				case 4:
				{
					if (Math::RandIntMinMax(0, 100) < 20)
					{
						Create::TileEntityCreator(TileEntity::RUNE_SPAWNER, Vector3((x + (row * 7)) + 100, y + (section * 7) + 100, 0), Vector3(0.5, 1, 1), true, true, true, i, true);
					}
				}
				break;
				case 5:
				{
					if (Math::RandIntMinMax(0, 100) < 100)
					{
						Create::TileEntityCreator(TileEntity::TOP_PLATFORM, Vector3((x + (row * 7)) + 100, ((y + (section * 7)) + 0.35) + 100, 0), Vector3(1, 0.3, 1), true, true, true, i, true);
					}
					else
						temp[i]->SetTilesInArray(x, y, 0);
				}
				break;
				default:
					break;
				}
			}
		}

		++row;

		if ((i + 1) % boss_map_database[level]->GetSizeOfLevel() == 0)
		{
			row = 0;
			++section;
		}
	}

	for (int y = 0; y < boss_map_database[level]->GetSizeOfLevel() * boss_map_database[level]->GetSizeOfTileSet(); ++y)
	{
		for (int x = 0; x < boss_map_database[level]->GetSizeOfLevel() * boss_map_database[level]->GetSizeOfTileSet(); ++x)
		{
			if (y == 0 || y == (boss_map_database[level]->GetSizeOfLevel() * boss_map_database[level]->GetSizeOfTileSet()) - 1)
			{
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3(x + 100, (y + ((y == 0) ? -1 : 1)) + 100, 0), Vector3(1, 1, 1), true, true, true, boss_map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()), true);
				if (x == 0 || x == (boss_map_database[level]->GetSizeOfLevel() * boss_map_database[level]->GetSizeOfTileSet()) - 1)
					Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3((x + ((x == 0) ? -1 : 1)) + 100, y + 100, 0), Vector3(1, 1, 1), true, true, true, boss_map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()), true);

			}
			else if (x == 0 || x == (boss_map_database[level]->GetSizeOfLevel() * boss_map_database[level]->GetSizeOfTileSet()) - 1)
				Create::TileEntityCreator(TileEntity::SOLID_BLOCK, Vector3((x + ((x == 0) ? -1 : 1)) + 100, y + 100, 0), Vector3(1, 1, 1), true, true, true, boss_map_database[level]->ReturnTileViaPos(Vector3(x, y, 0), Player::GetInstance()->GetIsFightingBoss()), true);

		}
	}

	GenerateMapArray(level);

	/*if (enemy_temp_list.size() > 0)
	{
		for (map<int, vector<Vector3>>::iterator it = enemy_temp_list.begin(); it != enemy_temp_list.end(); ++it)
		{
			for (vector<Vector3>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				Create::Enemy(EnemyBase::ENEMY_TYPE::E_MELEE, Vector3(it2->x, it2->y, it2->z + 0.1), Vector3(1, 1, 1), true, false, false, it->first, true);
			}
		}
	}*/
}

void MapManager::DeleteAllLevels()
{
	for (int i = 0; i < map_database.size(); ++i)
	{
		delete map_database[i];
		map_database[i] = nullptr;
	}

	for (int i = 0; i < boss_map_database.size(); ++i)
	{
		delete boss_map_database[i];
		boss_map_database[i] = nullptr;
	}

	map_database.clear();
	boss_map_database.clear();

	for (int i = 0; i < (16 * 7) + (16 * 7); ++i)
	{
		delete [i]m_map_array;
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
	int size = (16 * 7) + (16 * 7);

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
				if (temp[i]->GetTilesArray()[y][x] == 0 || temp[i]->GetTilesArray()[y][x] == 4 || temp[i]->GetTilesArray()[y][x] == 6 || temp[i]->GetTilesArray()[y][x] == 7 || temp[i]->GetTilesArray()[y][x] == 8)
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

	boss_map_database[level]->GetLevelLayOut(temp);

	section = 0, row = 0;
	for (int i = 0; i < temp.size(); ++i)
	{
		for (int y = 0; y < boss_map_database[level]->GetSizeOfTileSet(); ++y)
		{
			for (int x = 0; x < boss_map_database[level]->GetSizeOfTileSet(); ++x)
			{
				if (temp[i]->GetTilesArray()[y][x] == 0 || temp[i]->GetTilesArray()[y][x] == 4 || temp[i]->GetTilesArray()[y][x] == 6 || temp[i]->GetTilesArray()[y][x] == 7 || temp[i]->GetTilesArray()[y][x] == 8)
				{
					m_map_array[y + (section * 7) + 100][x + (row * 7) + 100] = 0;
				}
				else
					m_map_array[y + (section * 7) + 100][x + (row * 7) + 100] = temp[i]->GetTilesArray()[y][x];
			}
		}

		++row;

		if ((i + 1) % boss_map_database[level]->GetSizeOfLevel() == 0)
		{
			row = 0;
			++section;
		}
	}

	//for (int i = 0; i < 16 * 7; ++i)
	//{
	//	for (int j = 0; j < 16 * 7; ++j)
	//	{
	//		std::cout << m_map_array[i][j];
	//	}

	//	std::cout << std::endl;
	//}
}

vector<Vector3> MapManager::GetAllPlayerStartingPos()
{
	return player_starting_pos;
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
	int current_tile_ID, 
	bool is_boss_room)
{
	switch (block_type)
	{

	case TileEntity::SOLID_BLOCK:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("solid_block");

		if (Math::RandIntMinMax(0, 100) < 10)
			modelMesh = MeshList::GetInstance()->GetMesh("solid_block_cracked");

		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::LADDER:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("ladder_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::LADDERWITHPLATFORM:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("platform_ladder_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::TOP_PLATFORM:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("platform_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::RUNE_SPAWNER:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("rune_spawner_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::BOSS_DOOR:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("door_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	case TileEntity::DOOR_EXIT:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("exit_door_block");
		if (modelMesh == nullptr)
			return nullptr;

		TileEntitySolidBlock* result = new TileEntitySolidBlock(modelMesh, _position, _scale, have_collider, have_physic, current_tile_ID, is_static, block_type);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);

		return result;
	}
	default:
		return nullptr;
	}
}
