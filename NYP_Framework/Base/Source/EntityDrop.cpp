#include "EntityDrop.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"

void EntityDrop::Update(double dt)
{
	//Update sprite position
	animation->SetPosition(position);
	animation->SetScale(Vector3(1, 1, 1));
	animation->Update(dt);
}

void EntityDrop::Render()
{
	this->animation->Render();
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

EntityDrop * Create::Rune(const Vector3 & _position, const Vector3 & _scale, int tile_ID)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh("quad");
	if (modelMesh == nullptr)
		return nullptr;

	EntityDrop* result = new EntityDrop(Player::GetInstance()->GetCurrentLevel(), modelMesh, _position, _scale, tile_ID);
	switch (result->GetRuneDrop()->getAttribute()[0]->m_element)
	{
	case ET_FIRE:
	{

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("fire_rune_item")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	case ET_AIR:
	{
		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("wind_rune_item")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	case ET_EARTH:
	{

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("earth_rune_item")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	case ET_WATER:
	{

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("water_rune_item")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	case ET_NONE:
	{

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("normal_rune_item")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	default:
		break;
	}
}
