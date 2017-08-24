#include "PlagueBoss.h"
#include "PlagueBubbleStrategy.h"
#include "PlagueChargeStrategy.h"
#include "PlagueProjectileStrategy.h"
#include "PlagueSpawnStrategy.h"
#include "PoisonGasBubble.h"
#include "PoisonHitbox.h"
#include "PoisonProjectile.h"
#include "ToxicGas.h"

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
    this->m_strats[STATE_CHARGE] = new PlagueChargeStrategy;

	for (int i = 0;i < 10;++i)
	{
		this->m_entitylist.push_back(Create::CreatePoisonGasBubbles("q", Vector3()));
		this->m_entitylist.push_back(Create::CreatePoisonHitbox("q",Vector3()));
		this->m_entitylist.push_back(Create::CreatePoisonProjectile("q",Vector3()));
		this->m_entitylist.push_back(Create::CreateToxicGas("q", Vector3()));
	}
	m_changestatetimer = 0;
	m_defchangestatetimer = 5;
	m_currstate = STATE_IDLE;
	this->size.Set(5, 5, 1);

	GenerateAABB(this->position);
	this->scale = Vector3(1, 1, 1);
}

void PlagueBoss::Update(double _dt)
{
	if (!m_currstate && m_freestate) //State Idle
	{
		this->m_changestatetimer -= _dt;
		if (m_changestatetimer < 0)
		{
			m_freestate = false;
			this->GetNextState();
		}
	}
	else if (m_currstate >= STATE_SUMMON && m_currstate <= STATE_CHARGE)
	{
		this->m_strats[m_currstate]->Update(_dt);
	}
	else
		m_currstate = STATE_IDLE;

	GenerateAABB(this->position);

}

void PlagueBoss::Render()
{
	Collision::Render();
}

bool PlagueBoss::CollisionResponse(GenericEntity *)
{
    return false;
}

bool PlagueBoss::GetNextState()
{
	//m_currstate = static_cast<PLAGUESTATES>(Math::RandIntMinMax(STATE_SUMMON, STATE_CHARGE));
	m_currstate = STATE_BUBBLE;
	return false;
}

GenericEntity * PlagueBoss::GetEntity(GenericEntity::OBJECT_TYPE _type)
{
	if(_type < PLAGUE_BUBBLE_OBJ || _type > PLAGUE_POISON_OBJ)
		return nullptr;

	for (std::vector<GenericEntity*>::iterator it = m_entitylist.begin();it != m_entitylist.end();++it)
	{
		GenericEntity* go = (*it);

		if (!go->GetActive() && go->type == _type)
			return go;
	}

	return nullptr;
}
