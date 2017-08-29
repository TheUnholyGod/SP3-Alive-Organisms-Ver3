#pragma once

#include "EnemyBase.h"

class EnemyMelee : public EnemyBase
{
public:
	EnemyMelee(
		Mesh* mesh,
		EnemyBase::ENEMY_TYPE enemy_type,
		Vector3 _position,
		Vector3 _scale,
		bool have_collider = true,
		bool have_physic = true,
		bool is_static = false);
	virtual ~EnemyMelee();

	virtual void Update(double _dt);
	virtual void Render();

	virtual bool CollisionResponse(GenericEntity*);
	
	enum AI_STATES
	{
		AI_PATROL, //Walks left & right
		AI_CHASE,  //Follows path to player
		AI_ATTACK, //Attacks the player
		AI_ATTACKING,
		AI_RETURN, //Returns to original spawn
		AI_STUNNED,//Stops the AI From doing anything for a set period

		AI_TOTAL
	}m_state;

	//AI States
	void FindPath(Coord2D _src, Coord2D _end);
	void Patrol();
	void Detect(double dt);
	void Move();
	void Attack();
	
private:
	double m_stunned_duration;
};