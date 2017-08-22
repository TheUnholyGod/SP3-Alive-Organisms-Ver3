///////////////////////////////////////////////////////////////////////////////
/*!
\file   EnemyBase.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for EnemyBase. Contain basic information of the enemy.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <future>
#include "Pathing.h"
#include "../GenericEntity.h"

class EnemyBase : public GenericEntity
{
public:
	enum ENEMY_TYPE
	{
		E_MELEE,
		E_RANGED,
		E_BOMBER,
		E_TOTAL
	} m_enemyType;

	EnemyBase(Mesh* enemyMesh) : GenericEntity(enemyMesh)
	{
		this->type = GenericEntity::ENEMY_OBJ;
		this->m_velocity = Vector3(0, 0, 0);
	}
	~EnemyBase() {};

	virtual void Update(double _dt);
	virtual void Render();
	virtual bool CollisionResponse(GenericEntity*);
	virtual void Move();
	
protected:
	double m_timeSinceLastUpdate;
	Vector3 m_velocity;
};

namespace Create
{
	EnemyBase* Enemy(const EnemyBase::ENEMY_TYPE enemy_type,
		const Vector3& _position,
		const Vector3& _scale,
		bool have_collider = false,
		bool have_physic = false,
		bool is_static = false);

};
