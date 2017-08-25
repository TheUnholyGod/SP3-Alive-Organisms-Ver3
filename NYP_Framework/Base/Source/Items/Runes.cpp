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
	//TODO: Randomise stats based on constructor
	switch (type)
	{
	case RT_OFFENSIVE:
		
		break;
	case RT_DEFENSIVE:

		break;
	case RT_UTILITY:

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
