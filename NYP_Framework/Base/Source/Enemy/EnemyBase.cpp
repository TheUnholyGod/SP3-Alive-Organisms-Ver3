///////////////////////////////////////////////////////////////////////////////
/*!
\file   EnemyBase.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for EnemyBase.
*/
///////////////////////////////////////////////////////////////////////////////
#include "EnemyBase.h"

#include "../SpriteEntity.h"

#include "EnemyMelee.h"
#include "EnemyMaggot.h"
#include "EnemyFlying.h"


void EnemyBase::Update(double _dt)
{	
}

void EnemyBase::Render()
{
}

bool EnemyBase::CollisionResponse(GenericEntity *)
{
	return false;
}

EnemyBase * Create::Enemy(const EnemyBase::ENEMY_TYPE enemy_type, 
	const Vector3 & _position, 
	const Vector3 & _scale, 
	bool have_collider, 
	bool have_physic, 
	bool is_static,
	int tile_ID,
	bool is_boss_room)
{
	switch (enemy_type)
	{
	case EnemyBase::ENEMY_TYPE::E_MELEE:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("enemy_1");
		if (modelMesh == nullptr)
			return nullptr;

		EnemyMelee* result = new EnemyMelee(modelMesh, enemy_type, _position, _scale);
		
		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("enemy_1")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 6, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}
		SpriteAnimation* sa2 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("enemy_1_atk")));
		if (sa2)
		{
			sa2->m_anim = new Animation();
			sa2->m_anim->Set(1, 5, 1, 1.0f, true);
			result->animation2 = new SpriteEntity(sa2);
		}

		EntityManager::GetInstance()->AddEntity(result, is_boss_room);
		return result;
	}
	case EnemyBase::ENEMY_TYPE::E_MAGGOT:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("explosion");
		if (modelMesh == nullptr)
			return nullptr;

		EnemyMaggot* result = new EnemyMaggot(modelMesh, enemy_type, _position, _scale);

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("maggot")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 12, 1, 0.6f, true);
			result->animation = new SpriteEntity(sa);
		}
		SpriteAnimation* sa2 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("maggot")));
		if (sa2)
		{
			sa2->m_anim = new Animation();
			sa2->m_anim->Set(1, 12, 1, 1.0f, true);
			result->animation2 = new SpriteEntity(sa2);
		}
		result->SetTileID(-1);
		EntityManager::GetInstance()->AddEntity(result, is_boss_room);
		return result;
	}
	case EnemyBase::ENEMY_TYPE::E_FLYING:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("enemy_2");
		if (modelMesh == nullptr)
			return nullptr;

		EnemyFlying* result = new EnemyFlying(modelMesh, enemy_type, _position, _scale);

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("enemy_2")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 3, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}
		SpriteAnimation* sa2 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("explosion")));
		if (sa2)
		{
			sa2->m_anim = new Animation();
			sa2->m_anim->Set(1, 24, 1, 1.0f, true);
			result->animation2 = new SpriteEntity(sa2);
		}

		EntityManager::GetInstance()->AddEntity(result, is_boss_room);
		return result;
	}
	default:
		break;
	}
}
