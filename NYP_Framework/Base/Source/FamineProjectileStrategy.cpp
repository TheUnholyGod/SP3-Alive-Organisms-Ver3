#include "FamineProjectileStrategy.h"
#include "ArchProjectile.h"

FamineProjectileStrategy::FamineProjectileStrategy()
{
}

FamineProjectileStrategy::~FamineProjectileStrategy()
{
}

void FamineProjectileStrategy::Init()
{
	for (int i = 0; i < 20; ++i)
	{
		ArchProjectile* arch_prj = dynamic_cast<ArchProjectile*>(this->m_parent->GetEntity(GenericEntity::FAMINE_PROJECTILE_OBJ));

		if (arch_prj)
		{
			projectile.push_back(arch_prj);
		}
	}
	m_deftotaltimer = 5;
	m_totaltimer = 0;
	m_defactiontimer = 1;
	m_actiontimer = 0;
	action = false;
	m_isdone = false;
}

void FamineProjectileStrategy::Update(double dt)
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
	if (action && projectile.size())
	{
		ArchProjectile* proj = projectile[projectile.size() - 1];
		proj->SetActive(true);
		proj->Init(this->m_parent->GetPosition(), Vector3(), Vector3(), this->m_parent);
		proj->SetTileID(-1);
		projectile.pop_back();
		this->action = false;
	}
}

