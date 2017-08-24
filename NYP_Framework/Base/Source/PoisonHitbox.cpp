#include "PoisonHitbox.h"

PoisonHitbox::PoisonHitbox(Mesh * _modelMesh) : Hitbox(_modelMesh)
{
}

PoisonHitbox::~PoisonHitbox()
{
}

void PoisonHitbox::Init()
{
}

void PoisonHitbox::Update(double _dt)
{
}

void PoisonHitbox::Render()
{
}

bool PoisonHitbox::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}

PoisonHitbox * Create::CreatePoisonHitbox(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, bool is_boss)
{
	return nullptr;
}
