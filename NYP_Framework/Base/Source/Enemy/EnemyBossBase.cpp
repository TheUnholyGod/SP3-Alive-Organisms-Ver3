#include "EnemyBossBase.h"

EnemyBossBase::EnemyBossBase() : GenericEntity(nullptr)
{
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
