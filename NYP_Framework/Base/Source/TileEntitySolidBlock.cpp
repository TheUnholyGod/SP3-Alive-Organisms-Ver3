#include "TileEntitySolidBlock.h"
#include "PlayerInfo\PlayerInfo.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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

void TileEntitySolidBlock::Render()
{
	if (block_type != DOOR_EXIT)
	{
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}
	else
	{
		if (Player::GetInstance()->GetIsKilledBoss())
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(position.x, position.y, position.z);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(modelMesh);
			modelStack.PopMatrix();
		}
	}
}

bool TileEntitySolidBlock::CollisionResponse(GenericEntity* entity)
{
	if (entity->type == GenericEntity::PLAYER_OBJ && this->type == GenericEntity::ENVIORNMENT_OBJ)
	{
		switch(block_type)
		{
		case BOSS_DOOR:
		{
			if (Player::GetInstance()->IsInteracting())
			{
				Player::GetInstance()->SetIsFightingBoss(true);
			}
			return true;
		}
		case DOOR_EXIT:
		{
			if (Player::GetInstance()->IsInteracting() && Player::GetInstance()->GetIsKilledBoss())
			{
				Player::GetInstance()->StartNextLevel();
			}
			return true;
		}
		default:
			return false;
		}
	}

	return false;
}
