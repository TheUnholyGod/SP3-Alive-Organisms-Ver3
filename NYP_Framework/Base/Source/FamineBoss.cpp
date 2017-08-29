#include "FamineBoss.h"
#include "FamineMeleeStrategy.h"
#include "FamineProjectileStrategy.h"
#include "ArchProjectile.h"
#include "Hitbox.h"

FamineBoss::FamineBoss() : m_strats(new FamineStrategy*[NUM_STATES])
{
	this->isStatic = false;
	type = ENEMY_OBJ;
}

FamineBoss::~FamineBoss()
{
}

void FamineBoss::Init()
{
	this->m_strats[STATE_ATTACK] = new FamineMeleeStrategy;
	this->m_strats[STATE_ATTACK]->SetParent(this);

	this->m_strats[STATE_PROJECTILE] = new FamineProjectileStrategy;
	this->m_strats[STATE_PROJECTILE]->SetParent(this);


	for (int i = 0; i < 5; ++i)
	{
		GenericEntity* hb = Create::HitboxEntity("quad", Vector3(), Vector3(1, 1, 0), nullptr, true);
		hb->type = GenericEntity::FAMINE_HITBOX_OBJ;
		this->m_entitylist.push_back(hb);
		GenericEntity* p = Create::CreateArchProjectile("q", Vector3(), Vector3(1, 1, 1), nullptr, true);
		p->type = FAMINE_PROJECTILE_OBJ;
		this->m_entitylist.push_back(p);
	}
	m_changestatetimer = 0;
	m_defchangestatetimer = 5;
	m_currstate = STATE_IDLE;
	this->size.Set(2, 2, 1);

	GenerateAABB(this->position);
	this->scale = Vector3(1, 1, 1);
	this->isStatic = false;
	this->m_bCollider = true;
	this->tile_ID = -1;
}

void FamineBoss::Update(double _dt)
{
	if (!m_currstate) //State Idle
	{
		this->m_changestatetimer -= _dt;
		if (m_changestatetimer < 0)
		{
			this->GetNextState();
		}
	}
	if (m_currstate >= STATE_ATTACK && m_currstate <= STATE_PROJECTILE)
	{
		this->m_strats[m_currstate]->Update(_dt);
		if (this->m_strats[m_currstate]->GetIsDone())
		{
			m_currstate = STATE_IDLE;
		}
	}

	GenerateAABB(this->position);

}

void FamineBoss::Render()
{
	Collision::Render();
}

bool FamineBoss::CollisionResponse(GenericEntity *)
{
	return false;
}

bool FamineBoss::GetNextState()
{
	m_currstate = static_cast<FAMINESTATES>(Math::RandIntMinMax(STATE_ATTACK, STATE_PROJECTILE));
	this->m_strats[m_currstate]->Init();
	return false;
}

GenericEntity * FamineBoss::GetEntity(GenericEntity::OBJECT_TYPE _type)
{
	if (_type < FAMINE_HITBOX_OBJ || _type > FAMINE_PROJECTILE_OBJ)
		return nullptr;

	for (std::vector<GenericEntity*>::iterator it = m_entitylist.begin(); it != m_entitylist.end(); ++it)
	{
		GenericEntity* go = (*it);

		if (go && !go->GetActive() && go->type == _type)
			return go;
	}

	return nullptr;
}
