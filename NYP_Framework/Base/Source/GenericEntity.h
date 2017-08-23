#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"

class Mesh;

class GenericEntity : public EntityBase, public Collision
{
public:
	enum OBJECT_TYPE {
		NONE = 0,
		PLAYER_OBJ,
		ENVIORNMENT_OBJ,
		ENTITY_OBJ,
		PROJECTILE_OBJ,
		ENEMY_OBJ,
		HITBOX_OBJ,

		TOTAL

	}type;

	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();

	virtual bool CollisionResponse(GenericEntity* ThatEntity);

	virtual void SetActive(bool _active);
	virtual bool GetActive();
	// Set the maxAABB and minAABB
	//void SetAABB(Vector3 maxAABB, Vector3 minAABB);

protected:
	bool m_active;
	Mesh* modelMesh;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H