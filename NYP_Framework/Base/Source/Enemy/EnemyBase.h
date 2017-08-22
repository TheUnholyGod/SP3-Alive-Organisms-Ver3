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
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../GenericEntity.h"
#include "Pathing.h"
#include "RenderHelper.h"
#include "StrategyBase.h"

class EnemyBase : public GenericEntity
{
public:
	friend StrategyBase;
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
		m_timeSinceLastUpdate = 0;
		m_path_finder.setHeuristic(Heuristic::manhattan);
		m_path_finder.setDiagonalMovement(false);
		m_path_finder.readMap(MapManager::GetInstance()->getMapArray());
		m_path_index = 0;
		isPathFound = false;
		strats = nullptr;
	}
	~EnemyBase() {};

	virtual void Update(double _dt);
	virtual void Render();
	virtual bool CollisionResponse(GenericEntity*);
	
protected:
	//PathFinding
	double m_timeSinceLastUpdate;
	Vector3 m_velocity;
	std::future<std::vector<Coord2D>> m_result;
	PathFinder m_path_finder;
	std::vector<Coord2D> m_path;
	int m_path_index;
	bool isPathFound;
	//Strategy
	StrategyBase* strats;
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
