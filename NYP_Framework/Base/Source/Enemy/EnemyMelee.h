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
		bool have_collider = false,
		bool have_physic = false,
		bool is_static = false);
	~EnemyMelee();

	void Update(double _dt);
	void Render();
	bool collisionResponse(GenericEntity*);

private:
	PathFinder m_pathing;
};