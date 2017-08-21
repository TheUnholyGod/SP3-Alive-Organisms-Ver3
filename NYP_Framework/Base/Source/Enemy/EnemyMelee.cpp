#include "EnemyMelee.h"

#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"


EnemyMelee::EnemyMelee(Mesh * mesh, 
	EnemyBase::ENEMY_TYPE enemy_type, 
	Vector3 _position, 
	Vector3 _scale, 
	bool have_collider, 
	bool have_physic, 
	bool is_static) : EnemyBase(mesh)
{
	this->SetPosition(_position);
	this->SetScale(_scale);
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position));
	this->SetCollider(have_collider);
	this->SetPhysic(have_physic);
	this->SetStatic(false);

	m_timeSinceLastUpdate = 0;
	m_path_finder.setDiagonalMovement(false);
	m_path_finder.readMap(MapManager::GetInstance()->getMapArray());
	m_path_index = 0;
}

EnemyMelee::~EnemyMelee()
{
}

void EnemyMelee::Update(double _dt)
{
	Vector3 playerpos = Player::GetInstance()->GetPosition();

	//Update enemy pos
	this->position += m_velocity * _dt;
	Move();

	m_timeSinceLastUpdate += _dt;

	if (m_timeSinceLastUpdate > 1 && !m_result.valid())
	{
		FindPath({ (int)(position.x + 0.5), (int)(position.y + 0.5)},
				{ (int)(playerpos.x + 0.5), (int)(playerpos.y + 0.5)});
		m_timeSinceLastUpdate = 0;
	}
	//Check if worker thread is done, if done, obtain result.
	if (m_result.valid())
	{
		if (m_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			m_path.clear();
			m_path = m_result.get();
			m_path_index = 0;
		}
	}

	if (!m_path.empty())
	{
		std::cout << "Path vector is not empty" << std::endl;
		for (int i = 0; i < m_path.size(); ++i)
		{	
			if(i == 0)
				std::cout << "Start";
			std::cout << "->(" << m_path[i].x << ", " << m_path[i].y << ")";
			if (i == m_path.size() - 1)
				std::cout << "->End";
		}
		std::cout << std::endl;
		//m_path.clear();
	}
	this->tile_ID = Player::GetInstance()->GetTileID();/*
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position);*/
}

void EnemyMelee::Render()
{
	/*Sprite animation?*/
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

bool EnemyMelee::collisionResponse(GenericEntity *)
{
	/*Collision response for this enemy*/
	return false;
}

void EnemyMelee::Move()
{
	m_velocity.SetZero();

	//Check if enemy is already at node
	if (!m_path.empty())
	{
		float dist = (position - Vector3(m_path[m_path_index].x, m_path[m_path_index].y, 0)).LengthSquared();
		if (dist <= 0.01)
		{
			std::cout << "Reached: (" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ")" << std::endl;
			if(m_path_index + 1 < m_path.size())
				++m_path_index;
		}
	}	

	//Dont move when path nodes are empty
	if (m_path.empty())	return;

	std::cout << "Traveling towards: (" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ")" <<std::endl;

	//Move up
	if (position.y < m_path[m_path_index].y) 
	{
		this->m_velocity += Vector3(0, 1, 0);
		//return;
	}
	//Move down
	if (position.y > m_path[m_path_index].y)
	{
		this->m_velocity += Vector3(0, -1, 0);
		//return;
	}
	//Move left
	if (position.x > m_path[m_path_index].x)
	{
		this->m_velocity += Vector3(-1, 0, 0);
		//return;
	}
	//Move right
	if (position.x < m_path[m_path_index].x)
	{
		this->m_velocity += Vector3(1, 0, 0);
		//return;
	}
}

void EnemyMelee::FindPath(Coord2D _src, Coord2D _end)
{
	m_result = std::async(std::launch::async, &PathFinder::findPath,
		&m_path_finder,
		Coord2D(_src),
		Coord2D(_end));
}
