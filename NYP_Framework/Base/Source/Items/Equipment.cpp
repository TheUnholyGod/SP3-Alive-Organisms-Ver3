///////////////////////////////////////////////////////////////////////////////
/*!
\file   Equipment.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for Equipment class.
*/
///////////////////////////////////////////////////////////////////////////////
#include "Equipment.h"

Equipment::Equipment()
{
}

Equipment::~Equipment()
{
}

std::vector<Runes*> Equipment::getRunes()
{
	return this->m_runes;
}

int Equipment::getMaxRunes()
{
	return this->m_max_runes;
}

void Equipment::setMaxRunes(int num_slot)
{
	this->m_max_runes = num_slot;
}

bool Equipment::addRune(Runes * new_rune)
{
	/*TODO: Write code lol*/
	return false;
}

bool Equipment::removeRune(RUNE_TYPE type)
{
	/*TODO: Write code lol*/
	return false;
}

bool Equipment::replaceRune(Runes * new_rune)
{
	/*TODO: Write code lol*/
	return false;
}
