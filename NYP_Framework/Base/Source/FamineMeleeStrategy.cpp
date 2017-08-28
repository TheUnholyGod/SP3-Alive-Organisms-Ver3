#include "FamineMeleeStrategy.h"
#include "Hitbox.h"
#include "FamineBoss.h"
#include "PlayerInfo\PlayerInfo.h"

FamineMeleeStrategy::FamineMeleeStrategy()
{
}

FamineMeleeStrategy::~FamineMeleeStrategy()
{
}

void FamineMeleeStrategy::Init()
{
	GenericEntity* ge = this->m_parent->GetEntity(GenericEntity::FAMINE_HITBOX_OBJ);
	if (ge)
		m_meleeatt = dynamic_cast<Hitbox*>(ge);

	m_deftotaltimer = 2;
	m_totaltimer = 0;
	m_defactiontimer = 1;
	m_actiontimer = 0;
	action = false;
	m_isdone = false;
}

void FamineMeleeStrategy::Update(double dt)
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
	if (action)
	{
		Vector3 hbpos = this->m_parent->GetPosition();
		if (Player::GetInstance()->GetPosition().x < this->m_parent->GetPosition().x)
			hbpos.x -= 1;
		else 
			hbpos.x += 1;

		m_meleeatt->Init(true);
		m_meleeatt->SetPosition(hbpos);
		m_meleeatt->SetSize(Vector3(1, 1, 0));
		m_meleeatt->GenerateAABB(m_meleeatt->GetPosition());
		this->action = false;
	}
}

