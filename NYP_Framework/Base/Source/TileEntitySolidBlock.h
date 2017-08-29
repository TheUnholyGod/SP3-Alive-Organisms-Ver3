#ifndef _TILE_ENTITY_SOLID_BLOCK_H_
#define _TILE_ENTITY_SOLID_BLOCK_H_

#include "TileEntity.h"

class TileEntitySolidBlock : public TileEntity
{
public:
	TileEntitySolidBlock(Mesh*, Vector3, Vector3, bool, bool, int, bool, TileEntity::BLOCK_TYPE);
	virtual ~TileEntitySolidBlock() {}

	void Update(double);
	void Render();
	bool CollisionResponse(GenericEntity*);
};

#endif