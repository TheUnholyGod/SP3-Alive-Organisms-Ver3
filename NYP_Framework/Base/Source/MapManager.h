#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_

#include <map>

#include "Level.h"
#include "TileEntity.h"
#include "SingletonTemplate.h"

using std::map;

class MapManager : public Singleton<MapManager>
{

private:
	friend Singleton <MapManager>;

	MapManager() {}
	~MapManager() {}

	map<int, Level*> map_database;
	map<int, Level*> boss_map_database;
	int** m_map_array; //Array view
	//int** new_map_array; //Corrected map view

public:
	void Init();
	void GenerateBlocks(int);
	void DeleteAllLevels();
	
	//Return the entire map in array
	int** getMapArray();
	//Generate MapArray
	void GenerateMapArray(int);

	Level* GetLevel(int);
};


namespace Create
{
	TileEntity* TileEntityCreator(const TileEntity::BLOCK_TYPE block_type,
		const Vector3& _position,
		const Vector3& _scale, 
		bool have_collider, 
		bool have_physic,
		bool is_static,
		int current_tile_ID);

};

#endif