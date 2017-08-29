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
#include "../PlayerInfo/PlayerInfo.h"

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
	if (m_runes.size() < m_max_runes)
	{
		bool isDuplicate = false;
		for (size_t i = 0; i < m_runes.size(); ++i)
		{
			if (m_runes[i]->getType() == new_rune->getType())
			{
				isDuplicate = true;
				break;
			}
		}
		if (isDuplicate == false)
		{
			m_runes.push_back(new_rune);
			return true;
		}
	}
	return false;
}

bool Equipment::removeRune(RUNE_TYPE type)
{
	for (size_t i = 0; i< m_runes.size(); ++i)
	{
		if (m_runes[i]->getType() == type)
			m_runes.erase(m_runes.begin() + i);
	}
	return false;
}

Runes* Equipment::replaceRune(Runes * new_rune)
{
	for (size_t i = 0; i < m_runes.size(); ++i)
	{
		if (m_runes[i]->getType() == new_rune->getType())
		{
			Runes* temp = m_runes[i];
			m_runes[i] = new_rune;
			return temp;
		}
	}
	//If no rune of the same type is found, add it in
	addRune(new_rune);
}
