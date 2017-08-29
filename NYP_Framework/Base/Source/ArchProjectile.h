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
	}

	void Init(Vector3 _pos = Vector3(0, 0, 0), Vector3 _vel = (0, 0, 0), Vector3 _dir = Vector3(0, 0, 0), bool _isplayer = true);

	void Update(double dt);

	bool CollisionResponse(GenericEntity*);
};

#endif