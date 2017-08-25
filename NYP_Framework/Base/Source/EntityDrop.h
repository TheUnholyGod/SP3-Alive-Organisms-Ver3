#ifndef _ENTITY_DROP_H_
#define _ENTITY_DROP_H_

#include "GenericEntity.h"
#include "../Source/Items/Runes.h"

class SpriteEntity;

class EntityDrop : public GenericEntity
{
protected:
	Runes* m_runeDrop;

public:
	EntityDrop(int level, Mesh* tile_mesh, Vector3 _pos, Vector3 _scale, int tile_ID_input) : GenericEntity(tile_mesh)
	{
		position = _pos;
		scale = _scale;
		tile_ID = tile_ID_input;
		m_runeDrop = new Runes(level, static_cast<RUNE_TYPE>(Math::RandIntMinMax(0, RT_TOTAL - 1)));
		m_bCollider = true;
		m_bPhysic = false;
		isStatic = false;
	}

	void Update(double _dt);
	void Render();

	bool CollisionResponse(GenericEntity* ThatEntity);
	Runes* GetRuneDrop();

	SpriteEntity* animation;
};

namespace Create
{
	EntityDrop* Rune(
		const Vector3& _position,
		const Vector3& _scale,
		int tile_ID = 0);

};

#endif