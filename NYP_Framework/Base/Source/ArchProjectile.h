#ifndef _ARCH_PROJECTILE_H_
#define _ARCH_PROJECTILE_H_

#include "Projectile.h"

class ArchProjectile : public Projectile
{
private:
	float m_gravity;

public:
	ArchProjectile(Mesh* _mesh, Vector3 _velocity) : Projectile(_mesh)
	{
		type = FAMINE_PROJECTILE_OBJ;
	}

	void Init(Vector3 _pos = Vector3(0, 0, 0), Vector3 _vel = (0, 0, 0), Vector3 _dir = Vector3(0, 0, 0), bool _isplayer = true);

	void Update(double dt);
	void Render();

	bool CollisionResponse(GenericEntity*);
};

namespace Create
{
	ArchProjectile* CreateArchProjectile(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		Ranged* _parent = nullptr,
		bool is_boss_room = false);
};

#endif