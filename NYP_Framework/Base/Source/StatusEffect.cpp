#include "StatusEffect.h"
#include "GenericEntity.h"

StatusEffect::StatusEffect()
{
}

StatusEffect::~StatusEffect()
{
}

void StatusEffect::Update(double dt)
{
	this->m_timer += dt;
	if (m_timer > m_onestack_timer)
	{
		this->m_timer -= m_onestack_timer;
		this->m_totaltimer -= m_onestack_timer;
	}
}

void StatusEffect::AddStack()
{
	this->m_totaltimer += this->m_onestack_timer;
	this->m_stack += 1;
}

void StatusEffect::ApplyDamage()
{
	this->m_victim->ApplyDamage(m_dps * m_stack);
}
