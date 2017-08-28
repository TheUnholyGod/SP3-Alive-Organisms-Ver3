#include "PoisonHitbox.h"
#include "MeshList.h"
#include "EntityManager.h"

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
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	PoisonHitbox* result = new PoisonHitbox(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetTileID(-1);

	EntityManager::GetInstance()->AddEntity(result, is_boss);
	return result;
}
