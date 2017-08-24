#include "ToxicGas.h"

ToxicGas::ToxicGas(Mesh * _modelMesh) : Hitbox(m_mesh)
{
}

ToxicGas::~ToxicGas()
{
}

void ToxicGas::Init()
{
}

void ToxicGas::Update(double _dt)
{
}

void ToxicGas::Render()
{
}

bool ToxicGas::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}

ToxicGas * Create::CreateToxicGas(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, bool is_boss)
{
	return nullptr;
}
