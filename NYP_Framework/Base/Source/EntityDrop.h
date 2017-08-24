#ifndef _ENTITY_DROP_H_
#define _ENTITY_DROP_H_

#include "GenericEntity.h"
#include "../Source/Items/Runes.h"

class EntityDrop : public GenericEntity
{
protected:
	Runes* m_runeDrop;

public:
	EntityDrop(int level, Mesh* tile_mesh) : GenericEntity(tile_mesh)
	{
		m_runeDrop = new Runes(level, static_cast<RUNE_TYPE>(Math::RandIntMinMax(0, 2)));
	}

	void Update(double _dt);

	bool CollisionResponse(GenericEntity* ThatEntity);
	Runes* GetRuneDrop();
};

#endif