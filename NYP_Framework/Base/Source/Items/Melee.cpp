#include "Melee.h"
#include "../Hitbox.h"

Melee::Melee()
	:
    m_hitbox(new Hitbox*[NUM_COMBO])
{
}

Melee::Melee(int level, RUNE_TYPE type)
    : m_hitbox(new Hitbox*[NUM_COMBO])
{
	//TODO: Randomise stats based on constructor
}

Melee::~Melee()
{
}

Hitbox* Melee::GetCurrentHitbox()
{
	return this->m_hitbox[m_curr_combo];
}


