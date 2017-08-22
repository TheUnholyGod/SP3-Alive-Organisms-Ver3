#pragma once

#include "EnemyBase.h"
#include "StrategyBase.h"

class EnemyMelee : public EnemyBase
{
public:
	friend StrategyBase;
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
	void Move();
	void FindPath(Coord2D _src, Coord2D _end);

private:
	PathFinder m_path_finder;
	std::vector<Coord2D> m_path;
	int m_path_index;
	std::future<std::vector<Coord2D>> m_result;
	bool isPathFound;
};