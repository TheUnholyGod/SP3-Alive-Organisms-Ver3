#include "ArchProjectile.h"

#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

void ArchProjectile::Update(double dt)
{
	position.y -= m_gravity * dt;

	if (!m_active)
		return;
	this->position += velocity * dt;
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->GenerateAABB(this->position);

	if (position.y < 0)
	{
		this->SetIsDone(true);
		this->m_active = false;
		this->m_bCollider = false;
		this->tile_ID = -1;
	}
}

bool ArchProjectile::CollisionResponse(GenericEntity * ThatEntity)
{

	return false;
}
