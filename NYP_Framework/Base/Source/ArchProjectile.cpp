#include "ArchProjectile.h"

#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"

void ArchProjectile::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir, bool _isplayer)
{
	this->position = _pos;
	this->velocity = Vector3(Math::RandFloatMinMax(-8.0f, -1.0f), Math::RandFloatMinMax(1.0f, 15.0f), 0);
	this->direction = _dir;
	this->tile_ID = -1;
	this->isStatic = false;
	this->m_bCollider = true;
	this->size.Set(0.5f, 0.5f, 1);
	this->type = PROJECTILE_OBJ;
	this->m_active = true;
	this->m_isplayer = _isplayer;
	this->m_dmg = 5;
	m_gravity = 9.8f;
}

void ArchProjectile::Update(double dt)
{

	if (!m_active)
		return;

	if (position.y < 100)
	{
		this->m_active = false;
		this->m_bCollider = false;
		this->tile_ID = -1;
	}

	this->velocity.y -= m_gravity * dt;

	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->position += this->velocity * dt;
	this->GenerateAABB(this->position);
}

void ArchProjectile::Render()
{
	if (!m_active)
		return;

	//Collision::Render();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(this->position.x, this->position.y, this->position.z);
	modelStack.Scale(this->scale.x, this->scale.y, this->scale.z);
	RenderHelper::RenderMesh(this->modelMesh);
	modelStack.PopMatrix();
}

bool ArchProjectile::CollisionResponse(GenericEntity * ThatEntity)
{

	if (ThatEntity->type == PLAYER_OBJ)
	{
		Player::GetInstance()->ApplyDamage(5);
		this->m_active = false;
		this->m_bCollider = false;
	}
	return false;
}

ArchProjectile * Create::CreateArchProjectile(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent, bool is_boss_room)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ArchProjectile* result = new ArchProjectile(modelMesh,Vector3());
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetTileID(-1);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}
