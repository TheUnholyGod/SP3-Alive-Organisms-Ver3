///////////////////////////////////////////////////////////////////////////////
/*!
\file   Runes.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for Runes class.
*/
///////////////////////////////////////////////////////////////////////////////
#include "Runes.h"
#include "MyMath.h"

Runes::Runes() 
	: m_level(0),
	m_attributes(0), 
	m_modifier(nullptr)
{
}

Runes::Runes(int level, RUNE_TYPE type)
{
	m_modifier = new ItemModifier;

	switch (type)
	{
	case RT_OFFENSIVE:
	{
		Attribute *temp_attribute = new Attribute;
		if (Math::RandIntMinMax(1, 100) <= 40)
		{
			temp_attribute->m_type = AT_SPEED;
			temp_attribute->m_ATvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

			temp_attribute->m_element = static_cast<ELEMENTAL_TYPE>(Math::RandIntMinMax(0, ET_TOTAL - 1));
			temp_attribute->m_ETvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		}
		else
		{
			temp_attribute->m_type = AT_ATTACK;
			temp_attribute->m_ATvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

			temp_attribute->m_element = static_cast<ELEMENTAL_TYPE>(Math::RandIntMinMax(0, ET_TOTAL - 1));
			temp_attribute->m_ETvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		}

		m_attributes.push_back(temp_attribute);
		m_modifier->addModifier(static_cast<ItemModifier::MODIFIER_TYPE>(Math::RandIntMinMax(0, ItemModifier::MT_TOTAL - 1)));

	}
		break;
	case RT_DEFENSIVE:
	{
		Attribute *temp_attribute = new Attribute;
		
		temp_attribute->m_type = AT_DEFEND;
		temp_attribute->m_ATvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		temp_attribute->m_element = static_cast<ELEMENTAL_TYPE>(Math::RandIntMinMax(0, ET_TOTAL - 1));
		temp_attribute->m_ETvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		m_attributes.push_back(temp_attribute);
		m_modifier->addModifier(static_cast<ItemModifier::MODIFIER_TYPE>(Math::RandIntMinMax(0, ItemModifier::MT_TOTAL - 1)));

	}
		break;
	case RT_UTILITY:
	{
		Attribute *temp_attribute = new Attribute;
		
		temp_attribute->m_type = AT_SPEED;
		temp_attribute->m_ATvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		temp_attribute->m_element = static_cast<ELEMENTAL_TYPE>(Math::RandIntMinMax(0, ET_TOTAL - 1));
		temp_attribute->m_ETvalue = Math::RandIntMinMax(1, ((level + 1) > 5) ? 5 : (level + 1));

		m_attributes.push_back(temp_attribute);
		m_modifier->addModifier(static_cast<ItemModifier::MODIFIER_TYPE>(Math::RandIntMinMax(0, ItemModifier::MT_TOTAL - 1)));
	}
		break;
	default:
		break;
	}
}

Runes::~Runes()
{
}

void Runes::setLevel(int lv)
{
	this->m_level = lv;
}

int Runes::getLevel()
{
	return this->m_level;
}

void Runes::setType(RUNE_TYPE type)
{
	this->m_type = type;
}

RUNE_TYPE Runes::getType()
{
	return this->m_type;
}

void Runes::addAttribute(Attribute *a)
{
	this->m_attributes.push_back(a);
}

std::vector<Attribute*> Runes::getAttribute()
{
	return this->m_attributes;
}
