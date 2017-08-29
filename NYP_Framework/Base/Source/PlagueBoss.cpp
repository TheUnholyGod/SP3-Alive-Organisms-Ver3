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
#include "SpriteEntity.h"

PlagueBoss::PlagueBoss() : m_strats(new PlagueStrategy*[NUM_STATES])
{
	this->isStatic = false;
	type = ENEMY_OBJ;

}

PlagueBoss::PlagueBoss(Mesh * _mesh) : m_strats(new PlagueStrategy*[NUM_STATES])
{
    this->modelMesh = _mesh;
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

	SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("plagueboss")));
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 4, 1, 1.f, true);
		this->idle = new SpriteEntity(sa);
	}
	SpriteAnimation* sa2 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("plagueboss")));
	if (sa2)
	{
		sa2->m_anim = new Animation();
		sa2->m_anim->Set(7, 12, 1, 1.0f, true);
		this->charge = new SpriteEntity(sa2);
	}
	SpriteAnimation* sa3 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("plagueboss")));
	if (sa3)
	{
		sa3->m_anim = new Animation();
		sa3->m_anim->Set(13, 18, 1, 1.f, true);
		this->action = new SpriteEntity(sa3);
	}
	SpriteAnimation* sa4 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("plagueboss")));
	if (sa4)
	{
		sa4->m_anim = new Animation();
		sa4->m_anim->Set(19, 21, 1, 1.0f, true);
		this->die = new SpriteEntity(sa4);
	}


	for (int i = 0;i < 20;++i)
	{
		this->m_entitylist.push_back(Create::CreatePoisonGasBubbles("bubble", Vector3(),Vector3(1,1,0),nullptr,true));
		this->m_entitylist.push_back(Create::CreateToxicGas("bubble_gas", Vector3(), Vector3(1, 1, 1), true));
		this->m_entitylist.push_back(Create::Enemy(EnemyBase::E_MAGGOT, Vector3(), Vector3(1, 1, 1), true, false, false, 0, true));
	}
	m_changestatetimer = 0;
	m_defchangestatetimer = 2.5;
	m_currstate = STATE_IDLE;
	this->size.Set(2, 2, 1);

	GenerateAABB(this->position);
	this->scale = Vector3(1, 1, 1);
	this->isStatic = false;
	this->m_bCollider = true;
	this->tile_ID = -1;
	this->m_dmg = 0.1f;
    this->m_health = 100;
}

void PlagueBoss::Update(double _dt)
{
	this->idle->SetPosition(position);
	this->idle->Update(_dt);

	this->charge->SetPosition(position);
	this->charge->Update(_dt);

	this->action->SetPosition(position);
	this->action->Update(_dt);

	this->die->SetPosition(position);
	this->die->Update(_dt);

	if (this->m_health < 0)
		this->m_currstate = STATE_DIE;

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
			this->m_changestatetimer = this->m_defchangestatetimer;
		}
	}

	if (m_health <= 0)
	{
		--m_health;
		OnDead();
	}

	GenerateAABB(this->position);

}

void PlagueBoss::Render()
{
	if (m_health > -10)
	{
		//Collision::Render();
		switch (m_currstate)
		{
		case STATE_IDLE:
			this->idle->Render();
			break;
		case STATE_CHARGE:
			this->charge->Render();
			break;
		case STATE_BUBBLE:
			this->action->Render();
			break;
		case STATE_SUMMON:
			this->action->Render();
			break;
		case STATE_DIE:
			this->die->Render();
			break;
		default:
			break;
		}
	}

}

bool PlagueBoss::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == PLAYER_OBJ)
	{
		Player* ThatEntity1 = dynamic_cast<Player*>(ThatEntity);
        if(this->m_currstate == STATE_CHARGE)
            ThatEntity1->TakeDamage(5);
        else
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
