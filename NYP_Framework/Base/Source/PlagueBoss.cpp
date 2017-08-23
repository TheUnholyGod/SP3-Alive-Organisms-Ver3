#include "PlagueBoss.h"
#include "PlagueBubbleStrategy.h"
#include "PlagueChargeStrategy.h"
#include "PlagueProjectileStrategy.h"
#include "PlagueSpawnStrategy.h"

PlagueBoss::PlagueBoss() : m_strats(new PlagueStrategy*[NUM_STATES])
{
}

PlagueBoss::~PlagueBoss()
{
}

void PlagueBoss::Init()
{
    this->m_strats[STATE_SUMMON] = new PlagueSpawnStrategy;
    this->m_strats[STATE_BUBBLE] = new PlagueBubbleStrategy;
    this->m_strats[STATE_PROJECTILE] = new PlagueProjectileStrategy;
    this->m_strats[STATES_CHARGE] = new PlagueChargeStrategy;

}

void PlagueBoss::Update(double _dt)
{
}

void PlagueBoss::Render()
{
}

bool PlagueBoss::CollisionResponse(GenericEntity *)
{
    return false;
}
