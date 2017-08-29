#pragma once

#include "EnemyBase.h"

class EnemyFlying : public EnemyBase
{
public:
	EnemyFlying(
		Mesh* mesh,
		EnemyBase::ENEMY_TYPE enemy_type,
		Vector3 _position,
		Vector3 _scale,
		bool have_collider = true,
		bool have_physic = true,
		bool is_static = false);
	virtual ~EnemyFlying();

	virtual void Update(double _dt);
	virtual void Render();

	virtual bool CollisionResponse(GenericEntity*);

	enum AI_STATES
	{
		AI_CHASE,  //Fly to player
		AI_ATTACK, //Attacks the player
		AI_ATTACKING, //Attacks the player
		AI_STUNNED,//Stops the AI From doing anything for a set period
		AI_IDLE,

		AI_TOTAL
	}m_state;

	//AI States
	void Detect(double dt);
	void Move();
	void Attack();

private:
	double m_stunned_duration;
};