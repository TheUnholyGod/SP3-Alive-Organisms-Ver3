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
	~EnemyMelee();

	void Update(double _dt);
	void Render();

	bool CollisionResponse(GenericEntity*);
	
	void FindPath(Coord2D _src, Coord2D _end);
	void Move();
	void Patrol();
	
private:

};