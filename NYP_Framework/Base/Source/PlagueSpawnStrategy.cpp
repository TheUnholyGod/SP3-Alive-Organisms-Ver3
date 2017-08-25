#include "PlagueSpawnStrategy.h"
#include "PlagueBoss.h"
#include "Enemy\EnemyMaggot.h"
#include "GenericEntity.h"

PlagueSpawnStrategy::PlagueSpawnStrategy()
{
}

PlagueSpawnStrategy::~PlagueSpawnStrategy()
{
}

void PlagueSpawnStrategy::Init()
{
	for (int i = 0;i < 5;++i)
	{
		EnemyMaggot* maggot = dynamic_cast<EnemyMaggot*>(this->m_parent->GetEntity(GenericEntity::PLAGUE_MAGGOT_OBJ));

		if (maggot)
		{
			m_maggots.push_back(maggot);
		}
	}
	m_deftotaltimer = 2.5f;
	m_totaltimer = 0;
	m_defactiontimer = 0.5f;
	m_actiontimer = 0;
	action = false;
	m_isdone = false;
}

void PlagueSpawnStrategy::Update(double dt)
{
	m_actiontimer += dt;
	m_totaltimer += dt;
	if (m_actiontimer > m_defactiontimer)
	{
		action = true;
		m_actiontimer -= m_defactiontimer;
	}
	if (m_totaltimer > m_deftotaltimer)
	{
		this->m_isdone = true;
	}
	if (action && m_maggots.size())
	{
		EnemyMaggot* maggot = m_maggots[m_maggots.size() - 1];
		maggot->Init(this->m_parent->GetPosition());
		m_maggots.pop_back();
		this->action = false;
	}
}
