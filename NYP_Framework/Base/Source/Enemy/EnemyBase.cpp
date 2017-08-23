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
	bool is_static)
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
			sa->m_anim->Set(1, 9, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}
		SpriteAnimation* sa2 = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("explosion")));
		if (sa2)
		{
			sa2->m_anim = new Animation();
			sa2->m_anim->Set(1, 24, 1, 1.0f, true);
			result->animation2 = new SpriteEntity(sa2);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	case EnemyBase::ENEMY_TYPE::E_BOMBER:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("explosion");
		if (modelMesh == nullptr)
			return nullptr;

		EnemyMelee* result = new EnemyMelee(modelMesh, enemy_type, _position, _scale);

		//Animation
		SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("explosion")));
		if (sa)
		{
			sa->m_anim = new Animation();
			sa->m_anim->Set(1, 24, 1, 1.0f, true);
			result->animation = new SpriteEntity(sa);
		}

		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	default:
		break;
	}
}
