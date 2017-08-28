#include "PlagueBoss.h"
#include "PlagueBubbleStrategy.h"
#include "PlagueChargeStrategy.h"
#include "PlagueProjectileStrategy.h"
#include "PlagueSpawnStrategy.h"
#include "PoisonGasBubble.h"
#include "PoisonHitbox.h"
#include "PoisonProjectile.h"
#include "ToxicGas.h"
#include "Enemy\EnemyMaggot.h"
#include "MapManager.h"

PlagueBoss::PlagueBoss() : m_strats(new PlagueStrategy*[NUM_STATES])
{
	this->isStatic = false;

}

PlagueBoss::~PlagueBoss()
{
}

void PlagueBoss::Init()
{
    this->m_strats[STATE_SUMMON] = new PlagueSpawnStrategy;
	this->m_strats[STATE_SUMMON]->SetParent(this);
    this->m_strats[STATE_BUBBLE] = new PlagueBubbleStrategy;
	this->m_strats[STATE_BUBBLE]->SetParent(this);
    this->m_strats[STATE_CHARGE] = new PlagueChargeStrategy;
	this->m_strats[STATE_CHARGE]->SetParent(this);


	for (int i = 0;i < 20;++i)
	{
		this->m_entitylist.push_back(Create::CreatePoisonGasBubbles("quad", Vector3(),Vector3(1,1,0),nullptr,true));
		//this->m_entitylist.push_back(Create::CreatePoisonHitbox("quad",Vector3()));
		//this->m_entitylist.push_back(Create::CreatePoisonProjectile("quad",Vector3()));
		this->m_entitylist.push_back(Create::CreateToxicGas("quad", Vector3(), Vector3(1, 1, 1), true));
		this->m_entitylist.push_back(Create::Enemy(EnemyBase::E_MAGGOT, Vector3(), Vector3(1, 1, 1), true, false, false, 0, true));
	}
	m_changestatetimer = 0;
	m_defchangestatetimer = 5;
	m_currstate = STATE_IDLE;
	this->size.Set(2, 2, 1);

	GenerateAABB(this->position);
	this->scale = Vector3(1, 1, 1);
	this->isStatic = false;
	this->m_bCollider = true;
	this->tile_ID = 0;
	this->m_dmg = 10;
}

void PlagueBoss::Update(double _dt)
{
	if (!m_currstate) //State Idle
	{
		this->m_changestatetimer -= _dt;
		if (m_changestatetimer < 0) 
		{
			this->GetNextState();
		}
	}
	if (m_currstate >= STATE_SUMMON && m_currstate <= STATE_CHARGE)
	{
		this->m_strats[m_currstate]->Update(_dt);
		if (this->m_strats[m_currstate]->GetIsDone())
		{
			m_currstate = STATE_IDLE;
		}
	}

	GenerateAABB(this->position);

}

void PlagueBoss::Render()
{
	Collision::Render();
}

bool PlagueBoss::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == PLAYER_OBJ)
	{
		Player* ThatEntity1 = dynamic_cast<Player*>(ThatEntity);
		ThatEntity1->TakeDamage(m_dmg);
	}
	return false;
}

bool PlagueBoss::GetNextState()
{
	m_currstate = static_cast<PLAGUESTATES>(Math::RandIntMinMax(STATE_SUMMON, STATE_CHARGE));
	this->m_strats[m_currstate]->Init();
	return false;
}

GenericEntity * PlagueBoss::GetEntity(GenericEntity::OBJECT_TYPE _type)
{
	if(_type < PLAGUE_BUBBLE_OBJ || _type > PLAGUE_POISON_OBJ)
		return nullptr;

	for (std::vector<GenericEntity*>::iterator it = m_entitylist.begin();it != m_entitylist.end();++it)
	{
		GenericEntity* go = (*it);

		if (go && !go->GetActive() && go->type == _type)
			return go;
	}

	return nullptr;
}
