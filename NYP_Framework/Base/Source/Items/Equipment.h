///////////////////////////////////////////////////////////////////////////////
/*!
\file   Equipment.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for Equipment class. Stores basic Equipment params.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ItemBase.h"
#include "Runes.h"

class Player;

class Equipment : public ItemBase
{
public:
	Equipment();
	virtual ~Equipment();


    virtual void Init(Player* _owner = nullptr) = 0;
    virtual void Action(int _actiontype) = 0;
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;

	/*Add to the runes of this weapon
	Returns true if same type rune doesn't exist
	Returns false if same type rune exists*/
	bool addRune(Runes* new_rune);

	/*Returns this equipment's rune vector*/
	std::vector<Runes*> getRunes();

protected:
	/*Returns the max rune slot of this weapon*/
	int getMaxRunes();
	/*Sets the maximum rune slot of this weapon*/
	void setMaxRunes(int num_slot);

	/*Removes the same type runes
	Returns true if removed successfully
	Returns false if no rune of the specified type exists*/
	bool removeRune(RUNE_TYPE type);

	/*Replaces the existing rune with this rune
	Returns replaced rune if replaced successfully. If runes doesn't exist, add it in.
	Returns nullptr if replace failed*/
	Runes* replaceRune(Runes* new_rune);

	size_t m_max_runes; //Maximum number of runes this equipment can store
	std::vector<Runes*> m_runes; //Equipment's runes

	Player* m_owner;
};

