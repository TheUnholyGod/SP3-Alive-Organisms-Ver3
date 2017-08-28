#include "PoisonProjectile.h"

PoisonProjectile * Create::CreatePoisonProjectile(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent, bool is_boss)
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
	this->position = _pos;
	this->velocity = _vel;
	this->direction = _dir;
}

void PoisonProjectile::Update(double _dt)
{
	this->position += this->velocity * (float)_dt;
}

void PoisonProjectile::Render()
{
	Collision::Render();
}

bool PoisonProjectile::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}
