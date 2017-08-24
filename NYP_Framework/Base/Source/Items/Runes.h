///////////////////////////////////////////////////////////////////////////////
/*!
\file   Runes.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for Runes class. Stores basic Runes params.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <vector>
#include "ItemModifier.h"

/*Enums to store the type of rune this is*/
enum RUNE_TYPE
{
	RT_OFFENSIVE = 0,
	RT_DEFENSIVE,
	RT_UTILITY,
	RT_TOTAL
};
/*Enums to store the base attribute that the rune has.*/
enum ATTRIBUTE_TYPE
{
	AT_ATTACK = 0,
	AT_DEFEND,
	AT_SPEED,
	AT_TOTAL
};

/*
	Enums to store the base element that the rune has.
		ET_NONE  - No elemental abilty in rune
		ET_FIRE  - Fire elemental ability in rune  (burning, fire projectile, et cetera)
		ET_WATER - Water elemental ability in rune (negate attack damage on chance, et cetera)
		ET_EARTH - Earth elemental ability in rune (on chance life steal, et cetera)
		ET_AIR   - Air elemental ability in rune   (knocking back, et cetera)
		ET_TOTAL
*/
enum ELEMENTAL_TYPE
{
	ET_NONE, 
	ET_FIRE,
	ET_WATER,
	ET_EARTH,
	ET_AIR,
	ET_TOTAL
};

/*
	Structure of attribute
	AT & ET values have a cap of 5
	Values: 
		- 1: +3%
		- 2: +6%
		- 3: +9%
		- 4: +12%
		- 5: +15%

	AT values boosts base stats by percentage								 (final value in nearest int)
	ET values boosts base elemental duration/damage per second by percentage (final value in nearest int)
*/
struct Attribute
{
	ATTRIBUTE_TYPE m_type;
	ELEMENTAL_TYPE m_element;

	int m_ATvalue,
		m_ETvalue;
};

class Runes
{
public:
	Runes();
	Runes(int level, RUNE_TYPE type);
	virtual ~Runes();

	/*Set the rune's level. Also uses this to generate rune's attributes*/
	void setLevel(int lv);
	/*Returns the level of this rune in int*/
	int getLevel();

	/*Set the rune's type. Also uses this to generate rune's attributes*/
	void setType(RUNE_TYPE type);
	/*Returns the type of this rune*/
	RUNE_TYPE getType();

	/*Adds new attribute into the rune*/
	void addAttribute(Attribute* a);
	/*Returns the entire attribute vector*/
	std::vector<Attribute*> getAttribute();

private:
	int m_level; //Rune Level
	std::vector<Attribute*> m_attributes; //Rune attributes vector
	ItemModifier *m_modifier; //Modifier that this rune provide
	RUNE_TYPE m_type; //Type of this rune. Used to generate attributes
};