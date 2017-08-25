#include "EntityDrop.h"
#include "PlayerInfo\PlayerInfo.h"

void EntityDrop::Update(double _dt)
{
}

bool EntityDrop::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == GenericEntity::PLAYER_OBJ)
	{
		if (Player::GetInstance()->IsInteracting())
		{
			//Set Rune into player weapon
		}
	}

	return false;
}

Runes* EntityDrop::GetRuneDrop()
{
	return m_runeDrop;
}
