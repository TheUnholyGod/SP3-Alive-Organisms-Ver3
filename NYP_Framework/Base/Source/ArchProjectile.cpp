#include "ArchProjectile.h"

#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

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

	if (position.y < 0)
	{
		this->SetIsDone(true);
		this->m_active = false;
		this->m_bCollider = false;
		this->tile_ID = -1;
	}

	this->velocity.y -= m_gravity * dt;

	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->position += this->velocity * dt;
	this->GenerateAABB(this->position);
}

bool ArchProjectile::CollisionResponse(GenericEntity * ThatEntity)
{

	return false;
}
