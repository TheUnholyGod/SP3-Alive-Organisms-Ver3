#include "Hitbox.h"
#include "Items\Melee.h"
#include "MeshList.h"
#include "EntityManager.h"

Hitbox::Hitbox(Mesh * _modelMesh) : GenericEntity(_modelMesh)
{
	type = HITBOX_OBJ;
	this->m_bCollider = true;
	this->isStatic = false;
}

Hitbox::~Hitbox()
{
}

void Hitbox::Init()
{
}

void Hitbox::Update(double _dt)
{
	if (this->m_parent->GetCurrentHitbox() != this)
		return;
}

void Hitbox::Render()
{
	if (this->m_parent->GetCurrentHitbox() != this)
		return;

	Collision::Render();
}

bool Hitbox::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == ENEMY_OBJ)
		ThatEntity->SetIsDone(true);
	return false;
}

void Hitbox::SetParent(Melee * _parent)
{
	this->m_parent = _parent;
}

Hitbox * Create::HitboxEntity(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Melee * _parent)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Hitbox* result = new Hitbox(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetParent(_parent);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
