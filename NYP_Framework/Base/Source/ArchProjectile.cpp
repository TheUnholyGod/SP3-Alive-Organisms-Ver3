#include "ArchProjectile.h"

#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

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

	float x_velocity = velocity.x;
	Vector3 target(0, 0, 0);
	Vector3 distance = target - this->position;
	float time_to_hit = distance.x / x_velocity;
	if (time_to_hit < 0)
		time_to_hit = -time_to_hit;
	float y_velocity = time_to_hit * 0.5f * m_gravity;
	this->velocity.Set(distance.Normalized().x * x_velocity, y_velocity);

	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->GenerateAABB(this->position);
}

bool ArchProjectile::CollisionResponse(GenericEntity * ThatEntity)
{

	return false;
}
