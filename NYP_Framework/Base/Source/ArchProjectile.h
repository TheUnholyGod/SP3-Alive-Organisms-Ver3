#ifndef _ARCH_PROJECTILE_H_
#define _ARCH_PROJECTILE_H_

#include "Projectile.h"

class ArchProjectile : public Projectile
{
private:
	float m_gravity;

public:
	ArchProjectile(Mesh* _mesh) : Projectile(_mesh)
	{
		m_gravity = .8f;
	}

	void Update(double dt);

	bool CollisionResponse(GenericEntity*);
};

#endif