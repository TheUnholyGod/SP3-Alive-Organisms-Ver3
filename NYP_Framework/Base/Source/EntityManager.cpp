#include "EntityManager.h"
#include "EntityBase.h"
#include "CollisionManager.h"
#include "SpriteAnimation.h"
#include "MeshBuilder.h"
#include "MapManager.h"
#include "PlayerInfo\PlayerInfo.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	Level *level = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel());
	std::vector<int> temp = level->ReturnSurroundingTilesViaCurrentTile(Player::GetInstance()->GetTileID());
	for (int i = 0; i <= temp.size(); ++i)
	{
		if (i == temp.size())
		{
			temp.push_back(-1);
			break;
		}

		if (temp[i] == -1)
			break;
	}
	std::list<EntityBase*>::iterator it;
	Vector3 original_player_MaxAABB = Player::GetInstance()->GetMaxAABB(), original_player_MinAABB = Player::GetInstance()->GetMinAABB();

	Player::GetInstance()->SetAABB(Vector3(Player::GetInstance()->GetMaxAABB().x + 4, Player::GetInstance()->GetMaxAABB().y + 4, Player::GetInstance()->GetMaxAABB().z),
		Vector3(Player::GetInstance()->GetMinAABB().x - 4, Player::GetInstance()->GetMinAABB().y - 4, Player::GetInstance()->GetMinAABB().z));

	if (!Player::GetInstance()->GetIsFightingBoss())
	{
		for (int i = 0; i < temp.size(); ++i)
		{
			for (it = m_entity_map_base[temp[i]].begin(); it != m_entity_map_base[temp[i]].end(); ++it)
			{
				if (temp[i] == -1 || CollisionManager::GetInstance()->CheckAABBCollision(Player::GetInstance(), *it))
				{
					if ((*it)->GetIsStatic())
						entity_list_full.push_back(*it);
					else
						entity_list_full.push_front(*it);
				}
			}
		}
	}
	else
	{
		temp.clear();
		temp.push_back(-1);
		temp.push_back(0);
		temp.push_back(1);
		temp.push_back(2);
		temp.push_back(3);

		for (int i = 0; i < temp.size(); ++i)
		{
			for (it = m_entity_boss_map_base[temp[i]].begin(); it != m_entity_boss_map_base[temp[i]].end(); ++it)
			{
					if ((*it)->GetIsStatic())
						entity_list_full.push_back(*it);
					else
						entity_list_full.push_front(*it);
				}
		}

		for (it = m_entity_map_base[-1].begin(); it != m_entity_map_base[-1].end(); ++it)
		{
			if ((*it)->GetIsStatic())
				entity_list_full.push_back(*it);
			else
				entity_list_full.push_front(*it);
		}
	}

	Player::GetInstance()->SetAABB(original_player_MaxAABB, original_player_MinAABB);

	for (it = entity_list_full.begin(); it != entity_list_full.end(); ++it)
	{
		if ((*it)->GetIsStatic())
			break;

		(*it)->Update(_dt);
	}

	CollisionManager::GetInstance()->Update(entity_list_full);

	if (!Player::GetInstance()->GetIsFightingBoss())
	{
		for (int i = 0; i < temp.size(); ++i)
		{
			it = m_entity_map_base[temp[i]].begin();

			while (it != m_entity_map_base[temp[i]].end())
			{
				/*	if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{
				++it;
				continue;
				}*/

				if ((*it)->IsDone())
				{
					std::list<EntityBase*>::iterator it3;
					it3 = entity_list_full.begin();

					while (it3 != entity_list_full.end())
					{
						if (*it == *it3)
						{
							it3 = entity_list_full.erase(it3);
							break;
						}
						++it3;
					}

					delete *it;
					it = m_entity_map_base[temp[i]].erase(it);
				}
				else
				{
					if ((*it)->GetTileID() != temp[i])
					{
						EntityBase* temp_entity = *it;
						it = m_entity_map_base[temp[i]].erase(it);
						m_entity_map_base[temp_entity->GetTileID()].push_back(temp_entity);
					}
					else
						++it;
				}
			}
		}
	}
	else
	{
		temp.clear();
		temp.push_back(-1);
		temp.push_back(0);
		temp.push_back(1);
		temp.push_back(2);
		temp.push_back(3);
		for (int i = 0; i < temp.size(); ++i)
		{
			it = m_entity_boss_map_base[temp[i]].begin();

			while (it != m_entity_boss_map_base[temp[i]].end())
			{
				/*	if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{
				++it;
				continue;
				}*/

				if ((*it)->IsDone())
				{
					std::list<EntityBase*>::iterator it3;
					it3 = entity_list_full.begin();

					while (it3 != entity_list_full.end())
					{
						if (*it == *it3)
						{
							it3 = entity_list_full.erase(it3);
							break;
						}
						++it3;
					}

					delete *it;
					it = m_entity_boss_map_base[temp[i]].erase(it);
				}
				else
					++it;
			}
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	std::list<EntityBase*>::reverse_iterator it;

	for (it = entity_list_full.rbegin(); it != entity_list_full.rend(); ++it)
	{
		(*it)->Render();
	}

	entity_list_full.clear();
}

//// Render the UI entities
//void EntityManager::RenderUI()
//{
//	// Render all entities UI
//	std::list<EntityBase*>::iterator it, end;
//	end = entityList.end();
//	for (it = entityList.begin(); it != end; ++it)
//	{
//		(*it)->RenderUI();
//	}
//}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool fighting_boss)
{
	if (!fighting_boss)
		m_entity_map_base[_newEntity->GetTileID()].push_back(_newEntity);
	else
		m_entity_boss_map_base[_newEntity->GetTileID()].push_back(_newEntity);
}

void EntityManager::GetAllBlocksWithinTileRadius(int tile_ID, vector<EntityBase*>& input_vector, bool is_boss)
{
	std::vector<int> temp = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnSurroundingTilesViaCurrentTile(tile_ID);
	std::list<EntityBase*>::iterator it;

	if (!is_boss)
	{
		for (int i = 0; i < temp.size(); ++i)
		{
			for (it = m_entity_map_base[temp[i]].begin(); it != m_entity_map_base[temp[i]].end(); ++it)
			{
				/*if (cross_path && temp.size() > 8)
				{
				if (i == 0 || i == 2 || i == 6 || i == 8)
				continue;
				}*/

				if ((*it)->GetIsStatic())
					input_vector.push_back(*it);
			}
		}
	}
	else
	{
		temp.clear();
		temp.push_back(-1);
		temp.push_back(0);
		temp.push_back(1);
		temp.push_back(2);
		temp.push_back(3);

		for (int i = 0; i < temp.size(); ++i)
		{
			for (it = m_entity_boss_map_base[temp[i]].begin(); it != m_entity_boss_map_base[temp[i]].end(); ++it)
			{
				/*if (cross_path && temp.size() > 8)
				{
				if (i == 0 || i == 2 || i == 6 || i == 8)
				continue;
				}*/

				if ((*it)->GetIsStatic())
					input_vector.push_back(*it);
			}
		}
	}
}

void EntityManager::GetAllBlocks(vector<EntityBase*>& input_vector)
{
	std::list<EntityBase*>::iterator it;
	std::map<int, std::list<EntityBase*>>::iterator it2 = m_entity_map_base.begin();
	for (; it2 != m_entity_map_base.end(); ++it2)
	{
		it = it2->second.begin();

		for (; it != it2->second.end(); ++it)
		{
			if ((*it)->GetIsStatic())
				input_vector.push_back(*it);
		}
	}
}

void EntityManager::GetAllBlocksInTileSet(int tile_ID, vector<EntityBase*>& input_vector)
{
	std::list<EntityBase*>::iterator it;
	for (it = m_entity_map_base[tile_ID].begin(); it != m_entity_map_base[tile_ID].end(); ++it)
	{
		if ((*it)->GetIsStatic())
			input_vector.push_back(*it);
	}
}

void EntityManager::GetActiveEntity(vector<EntityBase*>& input_vector)
{
	for (auto &it : entity_list_full)
	{
		input_vector.push_back(it);
	}
}

void EntityManager::ResetEntityBase()
{
	for (std::map<int, std::list<EntityBase*>>::iterator it = m_entity_map_base.begin(); it != m_entity_map_base.end(); ++it)
	{
		if (it->first == -1)
			continue;

		it->second.clear();
	}

	for (std::map<int, std::list<EntityBase*>>::iterator it = m_entity_boss_map_base.begin(); it != m_entity_boss_map_base.end(); ++it)
	{
		if (it->first == -1)
			continue;

		it->second.clear();
	}
}


// Remove an entity from this EntityManager
//bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
//{
//	 Find the entity's iterator
//	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);
//	
//	 find if the entity has a collider and remove from list
//	if (_existingEntity->HasCollider()) {
//		std::list<EntityBase*>::iterator findCollider = std::find(collisionList.begin(), collisionList.end(), _existingEntity);
//		if (findCollider != collisionList.end())
//			findCollider = collisionList.erase(findCollider);
//	}
//
//	 Delete the entity if found
//	if (findIter != entityList.end())
//	{
//		delete *findIter;
//		findIter = entityList.erase(findIter);
//		return true;	
//	}
//	 Return false if not found
//	return false;
//}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}
