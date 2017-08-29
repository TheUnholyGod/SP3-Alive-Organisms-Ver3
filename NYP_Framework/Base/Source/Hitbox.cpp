#include "Hitbox.h"
#include "Items\Melee.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"
#include "GraphicsManager.h"
#include "Mtx44.h"
#include "RenderHelper.h"

Hitbox::Hitbox(Mesh * _modelMesh) : GenericEntity(_modelMesh)
{
	type = HITBOX_OBJ;
	this->tile_ID = -1;
	this->m_active = false;
	this->m_hbdeftimer = 0.25;
}

Hitbox::~Hitbox()
{
}

void Hitbox::Init(bool _isboss)
{
	this->tile_ID = -1;
	this->m_bCollider = true;
	this->isStatic = false;
	this->m_active = true;
	this->m_dmg = 5;
	this->m_hbtimer = m_hbdeftimer;
}

void Hitbox::Update(double _dt)
{
	if (!this->m_active)
		return;
	m_hbtimer += _dt;
	if (m_hbtimer > m_hbdeftimer)
		this->m_active = false;
}

void Hitbox::Render()
{
	if (!this->m_active)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(this->position.x, this->position.y, this->position.z);
	modelStack.Scale(this->scale.x, this->scale.y, this->scale.z);
	RenderHelper::RenderMesh(this->modelMesh);
	modelStack.PopMatrix();
	
}

bool Hitbox::CollisionResponse(GenericEntity * ThatEntity)
{
	if (this->m_parent)
	{
		if (ThatEntity->type == ENEMY_OBJ || ThatEntity->type == PLAGUE_MAGGOT_OBJ)
		{
			ThatEntity->ApplyDamage(Player::GetInstance()->GetDamage());
			this->m_active = false;
			this->m_bCollider = false;
		}
	}
	else
	{
		if (ThatEntity->type == PLAYER_OBJ)
		{
			Player* ThatEntity1 = dynamic_cast<Player*>(ThatEntity);
			ThatEntity1->TakeDamage(m_dmg);
			this->m_active = false;
			this->m_bCollider = false;
		}
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
	result->SetTileID(-1);

	EntityManager::GetInstance()->AddEntity(result, _isboss);
	return result;
}
