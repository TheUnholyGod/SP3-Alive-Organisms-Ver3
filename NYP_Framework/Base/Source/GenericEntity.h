#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"

class Mesh;

class GenericEntity : public EntityBase, public Collision
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	enum OBJECT_TYPE {
		NONE = 0,
		PLAYER_OBJ,
		ENVIORNMENT_OBJ,
		ENTITY_OBJ,
		PROJECTILE_OBJ,

		TOTAL

	}type;

	virtual void Update(double _dt);
	virtual void Render();

	virtual bool CollisionResponse(GenericEntity* ThatEntity);

	// Set the maxAABB and minAABB
	//void SetAABB(Vector3 maxAABB, Vector3 minAABB);

protected:
	Mesh* modelMesh;
};

namespace Create
{
	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position, 
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H