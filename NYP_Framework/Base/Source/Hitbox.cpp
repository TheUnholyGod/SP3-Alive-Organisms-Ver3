#include "Hitbox.h"
#include "Items\Melee.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

Hitbox::Hitbox(Mesh * _modelMesh) : GenericEntity(_modelMesh)
{
	type = HITBOX_OBJ;
}

Hitbox::~Hitbox()
{
}

void Hitbox::Init(bool _isboss)
{
	if (!_isboss)

		this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position);
	else
		this->tile_ID = 0;
	this->m_bCollider = true;
	this->isStatic = false;
	this->m_active = true;
	
}

void Hitbox::Update(double _dt)
{
	if (this->m_parent->GetCurrentHitbox() != this || !this->m_active)
		return;
}

void Hitbox::Render()
{
	if (this->m_parent->GetCurrentHitbox() != this || !this->m_active)
		return;

	Collision::Render();
}

bool Hitbox::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == ENEMY_OBJ)
	{
		ThatEntity->SetIsDone(true);
		this->m_active = false;
	}
	return false;
}

void Hitbox::SetParent(Melee * _parent)
{
	this->m_parent = _parent;
}

Hitbox * Create::HitboxEntity(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Melee * _parent,bool _isboss)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Hitbox* result = new Hitbox(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetParent(_parent);
	EntityManager::GetInstance()->AddEntity(result, _isboss);
	return result;
}
