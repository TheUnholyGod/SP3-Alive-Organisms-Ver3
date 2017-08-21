///////////////////////////////////////////////////////////////////////////////
/*!
\file   ItemModifier.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for ItemModifier class. Stores basic ItemModifier params.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Modifier.h"

class ItemModifier
{
public:
	enum MODIFIER_TYPE
	{
		MT_BLEED = 0,
		MT_POISON,
		MT_TOTAL,
	};
	ItemModifier()
		: m_modifier(nullptr)
	{
		this->m_modifier = new Modifier();
	};
	~ItemModifier() {};

	/*Adds more modifiers*/
	bool addModifier(ItemModifier::MODIFIER_TYPE type);

private:
	IModifier* m_modifier;
};
