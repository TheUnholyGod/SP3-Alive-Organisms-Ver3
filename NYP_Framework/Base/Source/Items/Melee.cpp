#include "Melee.h"

Melee::Melee()
	:
    m_hitbox(new Collision*[NUM_COMBO])
{
}

Melee::Melee(int level, RUNE_TYPE type)
    : m_hitbox(new Collision*[NUM_COMBO])
{
	//TODO: Randomise stats based on constructor
}

Melee::~Melee()
{
}


