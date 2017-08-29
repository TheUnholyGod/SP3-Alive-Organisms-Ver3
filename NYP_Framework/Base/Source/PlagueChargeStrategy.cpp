#include "PlagueChargeStrategy.h"
#include "PoisonHitbox.h"
#include "PlagueBoss.h"
#include "PlayerInfo\PlayerInfo.h"


PlagueChargeStrategy::PlagueChargeStrategy()
{
}

PlagueChargeStrategy::~PlagueChargeStrategy()
{
}

void PlagueChargeStrategy::Init()
{
	for (int i = 0;i < 3;++i)
	{
		PoisonHitbox* poison = dynamic_cast<PoisonHitbox*>(this->m_parent->GetEntity(GenericEntity::PLAGUE_BUBBLE_OBJ));

		if (poison)
		{
			m_poison.push_back(poison);
		}
		else break;
	}
	m_deftotaltimer = 6;
	m_totaltimer = 0;
	m_defactiontimer = 1;
	m_actiontimer = 1;
	action = false;
	m_isdone = false;
	m_charges = 3;
}

void PlagueChargeStrategy::Update(double dt)
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
	if (action && m_charges)
	{
		action = false;
		this->m_velocity = (-this->m_parent->GetPosition() + Player::GetInstance()->GetPosition()).Normalized() * 10;
		--m_charges;
	}
	if (m_velocity.x && m_velocity.y)
	{
		Vector3 temp_pos = this->m_parent->GetPosition() + this->m_velocity * dt;
		//this->m_parent->SetPosition(this->m_parent->GetPosition() + this->m_velocity * dt);
		if (temp_pos.x > 100.5 && temp_pos.x < 111.5)
		{
			this->m_parent->SetPosition(Vector3(temp_pos.x, this->m_parent->GetPosition().y, this->m_parent->GetPosition().z));
			if (temp_pos.y > 100.5 && temp_pos.y < 111.5)
			{
				this->m_parent->SetPosition(Vector3(this->m_parent->GetPosition().x, temp_pos.y, this->m_parent->GetPosition().z));
			}
		}
	}
}
