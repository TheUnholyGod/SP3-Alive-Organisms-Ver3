#ifndef _TILE_ENTITY_H_
#define _TILE_ENTITY_H_

#include "GenericEntity.h"

class TileEntity : public GenericEntity
{
protected:
	Vector3 dir;

public:
	enum BLOCK_TYPE
	{
		SOLID_BLOCK = 0,
		LADDER, 
		LADDERWITHPLATFORM,
		TOP_PLATFORM,
		RUNE_SPAWNER,
		SOLID_SPAWNER,
		BOSS_DOOR,
		DOOR_EXIT,

		TOTAL
	} block_type;

	TileEntity(Mesh* tile_mesh) : GenericEntity(tile_mesh)
	{
		type = GenericEntity::ENVIORNMENT_OBJ;
	}

	virtual ~TileEntity();

	Vector3 GetDir();
	virtual void Update(double);
	virtual void Render();
	virtual bool CollisionResponse(GenericEntity*);

};

#endif