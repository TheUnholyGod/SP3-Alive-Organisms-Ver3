#include "PoisonProjectile.h"

PoisonProjectile * Create::CreatePoisonProjectile(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent)
{
	return nullptr;
}

PoisonProjectile::PoisonProjectile()
{
}

PoisonProjectile::PoisonProjectile(Mesh * _mesh)
{
}

PoisonProjectile::~PoisonProjectile()
{
}

void PoisonProjectile::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir)
{
}

void PoisonProjectile::Update(double _dt)
{
}

void PoisonProjectile::Render()
{
}

bool PoisonProjectile::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}
