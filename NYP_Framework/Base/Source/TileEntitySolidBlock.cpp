#include "TileEntitySolidBlock.h"
#include "PlayerInfo\PlayerInfo.h"
#include <iostream>

TileEntitySolidBlock::TileEntitySolidBlock(Mesh* mesh, Vector3 pos, Vector3 size, bool have_collider, bool have_physic, int current_tile_ID, bool is_static, TileEntity::BLOCK_TYPE type) : TileEntity(mesh)
{
	position = pos;
	scale = size;
	SetCollider(have_collider);
	m_bPhysic = have_physic;
	block_type = type;
	tile_ID = current_tile_ID;
	isStatic = is_static;

	if (type == TileEntity::TOP_PLATFORM)
	{
		dir.Set(0, 1, 0);
	}

	SetAABB(Vector3((pos.x + (size.x * 0.5)), (pos.y + (size.y * 0.5)), (pos.z + (size.z * 0.5))), Vector3((pos.x - (size.x * 0.5)), (pos.y - (size.y * 0.5)), (pos.z - (size.z * 0.5))));

}

void TileEntitySolidBlock::Update(double dt)
{
}

bool TileEntitySolidBlock::CollisionResponse(GenericEntity* entity)
{
	if (entity->type == GenericEntity::PLAYER_OBJ && this->type == GenericEntity::ENVIORNMENT_OBJ)
	{
		if (block_type == BOSS_DOOR)
		{
			if (Player::GetInstance()->IsInteracting())
			{
				Player::GetInstance()->SetIsFightingBoss(true);
			}
			//	std::cout << "[ COLLIDED PLAYER WITH BLOCK ]" << std::endl;
			return true;
		}
	}

	return false;
}
