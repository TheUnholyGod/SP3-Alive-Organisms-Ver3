#include "ToxicGas.h"
#include "PoisonGasBubble.h"
#include "MeshList.h"
#include "EntityManager.h"

ToxicGas::ToxicGas(Mesh * _modelMesh) : Hitbox(m_mesh)
{
}

ToxicGas::~ToxicGas()
{
}

void ToxicGas::Init(PoisonGasBubbles * _parent)
{
    this->m_active = true;
    this->position = _parent->GetPosition();
    this->m_bCollider = true;
    this->isStatic = false;
}

void ToxicGas::Update(double _dt)
{
}

void ToxicGas::Render()
{
	Collision::Render();
}

bool ToxicGas::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}

ToxicGas * Create::CreateToxicGas(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, bool is_boss)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ToxicGas* result = new ToxicGas(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetTileID(-1);

	EntityManager::GetInstance()->AddEntity(result, is_boss);
	return result;
}
