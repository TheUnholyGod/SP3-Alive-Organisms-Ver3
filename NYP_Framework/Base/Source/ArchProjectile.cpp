#include "ArchProjectile.h"

void ArchProjectile::Update(double dt)
{
	position.y -= m_gravity * dt;
}
