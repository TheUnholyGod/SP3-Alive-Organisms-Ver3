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
	std::list<EntityBase*>::iterator it;

	std::list<EntityBase*> entity_list_full;

	for (int i = 0; i < temp.size(); ++i)
	{
		for (it = m_entity_map_base[temp[i]].begin(); it != m_entity_map_base[temp[i]].end(); ++it)
		{
			if((*it)->GetIsStatic())
				entity_list_full.push_back(*it);
			else
				entity_list_full.push_front(*it);
		}
	}



	for (it = entity_list_full.begin(); it != entity_list_full.end(); ++it)
	{
		if ((*it)->GetIsStatic())
			break;

		(*it)->Update(_dt);
	}

	CollisionManager::GetInstance()->Update(entity_list_full);

	entity_list_full.clear();

	for (int i = 0; i < temp.size(); ++i)
	{
		it = m_entity_map_base[temp[i]].begin();

		while(it != m_entity_map_base[temp[i]].end())
		{
			if ((*it)->IsDone())
			{
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

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	Level *level = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel());
	std::vector<int> temp = level->ReturnSurroundingTilesViaCurrentTile(Player::GetInstance()->GetTileID());
	std::list<EntityBase*>::iterator it;

	for (int i = 0; i < temp.size(); ++i)
	{
		for (it = m_entity_map_base[temp[i]].begin(); it != m_entity_map_base[temp[i]].end(); ++it)
		{
			(*it)->Render();
		}
	}
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
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	m_entity_map_base[_newEntity->GetTileID()].push_back(_newEntity);
}

void EntityManager::GetAllBlocksWithinTileRadius(int tile_ID, vector<EntityBase*>& input_vector, bool cross_path)
{
	std::vector<int> temp = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnSurroundingTilesViaCurrentTile(tile_ID);
	std::list<EntityBase*>::iterator it;

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

void EntityManager::GetAllBlocks(vector<EntityBase*>& input_vector)
{
	std::list<EntityBase*>::iterator it;
	for (int i = 0; i < m_entity_map_base.size(); ++i)
	{
		it = m_entity_map_base[i].begin();

		for (it = m_entity_map_base[i].begin(); it != m_entity_map_base[i].end(); ++it)
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
