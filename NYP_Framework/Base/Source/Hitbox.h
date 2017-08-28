#ifndef _HITBOX_H
#define _HITBOX_H

#include "GenericEntity.h"

class Melee;

class Hitbox : public GenericEntity
{
public:
	Hitbox(Mesh* _modelMesh);
	virtual ~Hitbox();

	virtual void Init(bool _isboss);
	virtual void Update(double _dt);
	virtual void Render();

	virtual bool CollisionResponse(GenericEntity* ThatEntity);
	virtual void SetParent(Melee* _parent);
protected:
	Melee* m_parent;
	double m_hbtimer, m_hbdeftimer;
};

namespace Create
{
	Hitbox* HitboxEntity(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		Melee* _parent = nullptr,
		bool _isboss = false);
};

#endif