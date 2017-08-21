///////////////////////////////////////////////////////////////////////////////
/*!
\file   ItemModifier.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for ItemModifier class.
*/
///////////////////////////////////////////////////////////////////////////////
#include "ItemModifier.h"

bool ItemModifier::addModifier(ItemModifier::MODIFIER_TYPE type)
{
	/*TODO: Add more modifier types*/
	switch (type)
	{
	case ItemModifier::MT_BLEED:
	{
		this->m_modifier = new BleedModifier(*m_modifier);
		return true;
	}
	case ItemModifier::MT_POISON:
	{
		this->m_modifier = new PoisonModifier(*m_modifier);
		return true;
	}
	default:
		return false;
	}
}
