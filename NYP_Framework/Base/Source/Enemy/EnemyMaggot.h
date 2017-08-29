#pragma once

#include "EnemyBase.h"

class EnemyMaggot : public EnemyBase
{
public:
	EnemyMaggot(
		Mesh* mesh,
		EnemyBase::ENEMY_TYPE enemy_type,
		Vector3 _position,
		Vector3 _scale,
		bool have_collider = true,
		bool have_physic = true,
		bool is_static = false);
	virtual ~EnemyMaggot();

	virtual void Init(Vector3 pos);

	virtual void Update(double _dt);
	virtual void Render();

	bool CollisionResponse(GenericEntity*);

	enum AI_STATES
	{
		AI_CHASE,  //Follow player
		AI_ATTACK, //Attack the player
		AI_ATTACKING,

		AI_TOTAL
	}m_state;

	//AI State functions
	void Detect(double dt);
	void Move();
	void Attack();

private:

};