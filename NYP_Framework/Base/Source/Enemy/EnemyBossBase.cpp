#include "EnemyBossBase.h"
#include "../TileEntity.h"
#include "../MapManager.h"

EnemyBossBase::EnemyBossBase() : GenericEntity(nullptr)
{
	done_dead = false;
}

EnemyBossBase::~EnemyBossBase()
{
}

void EnemyBossBase::Init()
{
}

void EnemyBossBase::Update(double _dt)
{
}

void EnemyBossBase::Render()
{
}

bool EnemyBossBase::CollisionResponse(GenericEntity *)
{
    return false;
}

void EnemyBossBase::OnDead()
{
	if (!done_dead)
	{
		Create::TileEntityCreator(TileEntity::BOSS_DOOR, position, Vector3(1, 1.5, 1), true, true, true, 0, true);
		done_dead = true;
	}
}
