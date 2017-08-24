#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include <map>
#include <vector>

using std::vector;
using std::map;

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	/*void RenderUI();*/

	void AddEntity(EntityBase* _newEntity, bool fighting_boss = false);
	void GetAllBlocksWithinTileRadius(int, vector<EntityBase*>&, bool);
	void GetAllBlocks(vector<EntityBase*>&);
	void GetAllBlocksInTileSet(int, vector<EntityBase*>&);
	/*bool RemoveEntity(EntityBase* _existingEntity);*/

private:
	EntityManager();
	virtual ~EntityManager();

	map<int, std::list<EntityBase*>> m_entity_map_base;
	map<int, std::list<EntityBase*>> m_entity_boss_map_base;
	vector<EntityBase*> m_all_blocks_storage;


};

#endif // ENTITY_MANAGER_H