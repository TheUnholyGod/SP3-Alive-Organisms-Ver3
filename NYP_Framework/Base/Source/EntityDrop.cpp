#include "EntityDrop.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

void EntityDrop::Update(double dt)
{
	m_floating_timer += 10 * dt;

	position.y += cosf(m_floating_timer) * 0.0009;
}

bool EntityDrop::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == GenericEntity::PLAYER_OBJ)
	{
		if (Player::GetInstance()->IsInteracting())
		{
			// Open UI interface
			Player::GetInstance()->SetRuneToWeapon(true, m_runeDrop);
			SetIsDone(true);
		 
		}
	}

	return false;
}

Runes* EntityDrop::GetRuneDrop()
{
	return m_runeDrop;
}

void EntityDrop::SetModelMesh(Mesh* model_mesh)
{
	modelMesh = model_mesh;
}

EntityDrop* Create::Rune(const Vector3 & _position, const Vector3 & _scale, int tile_ID)
{

	Mesh* modelMesh = MeshList::GetInstance()->GetMesh("quad");
	if (modelMesh == nullptr)
		return nullptr;

	EntityDrop* result = new EntityDrop(Player::GetInstance()->GetCurrentLevel(), modelMesh, _position, _scale, tile_ID);
	switch (result->GetRuneDrop()->getAttribute()[0]->m_element)
	{
	case ET_FIRE:
	{

		modelMesh = MeshList::GetInstance()->GetMesh("fire_rune_item");

		if (modelMesh == nullptr)
			return nullptr;

		result->SetModelMesh(modelMesh);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case ET_AIR:
	{
		modelMesh = MeshList::GetInstance()->GetMesh("wind_rune_item");

		if (modelMesh == nullptr)
			return nullptr;

		result->SetModelMesh(modelMesh);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case ET_EARTH:
	{
		modelMesh = MeshList::GetInstance()->GetMesh("earth_rune_item");

		if (modelMesh == nullptr)
			return nullptr;

		result->SetModelMesh(modelMesh);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case ET_WATER:
	{

		modelMesh = MeshList::GetInstance()->GetMesh("water_rune_item");

		if (modelMesh == nullptr)
			return nullptr;

		result->SetModelMesh(modelMesh);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	case ET_NONE:
	{

		modelMesh = MeshList::GetInstance()->GetMesh("normal_rune_item");

		if (modelMesh == nullptr)
			return nullptr;

		result->SetModelMesh(modelMesh);
		EntityManager::GetInstance()->AddEntity(result);

		return result;
	}
	default:
		break;
	}
}
