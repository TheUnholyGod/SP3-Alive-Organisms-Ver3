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
	this->SetSize(_scale);
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position));
	this->SetCollider(have_collider);
	this->SetPhysic(have_physic);
	this->SetStatic(false);
}

EnemyMelee::~EnemyMelee()
{
}

void EnemyMelee::Update(double _dt)
{
	//Create AABB for collision
	this->GenerateAABB(this->position);

	//Update enemy pos
	this->position += m_velocity * _dt;
	
	//Iterate through the path vector and move through the terrain
	Move();

	//Increment the time since last update for regulated checks
	m_timeSinceLastUpdate += _dt;

	//Player pos to find the player
	Vector3 playerpos = Player::GetInstance()->GetPosition();

	if (m_timeSinceLastUpdate > 1 && !m_result.valid() && !isPathFound)
	{
		FindPath({ (int)(position.x + 0.5), (int)(position.y + 0.5)},
				{ (int)std::floor(playerpos.x + 0.5), (int)std::floor(playerpos.y + 0.5)});
		m_timeSinceLastUpdate = 0;
	}
	//Check if worker thread is done, if done, obtain result.
	if (m_result.valid() && !isPathFound)
	{
		if (m_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			m_path.clear();
			m_path = m_result.get();
			m_path_index = 0;
			if(!m_path.empty())
				isPathFound = true;
		}
	}
	//Update tileID for spatial partition
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position);
}

void EnemyMelee::Render()
{
	Collision::Render();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

bool EnemyMelee::CollisionResponse(GenericEntity *ThatEntity)
{
	/*Collision response for this enemy*/
	if (ThatEntity->type == PROJECTILE_OBJ)
	{
		ThatEntity->SetIsDone(true);
		this->SetIsDone(true);
	}
	return false;
}

void EnemyMelee::Move()
{
	m_velocity.SetZero();

	//Dont move when path nodes are empty
	if (m_path.empty())	return;

	//Check if enemy is already at node
	if (!m_path.empty())
	{
		Vector3 absPos = Vector3(std::abs(position.x), std::abs(position.y), std::abs(position.z));
		float dist;
		if (m_path_index + 1 < m_path.size())
			dist = (position - Vector3(m_path[m_path_index + 1].x, m_path[m_path_index + 1].y, 0)).Length();
		else
		{
			std::cout << "Reached the Destination!" << std::endl;
			m_path.clear();
			isPathFound = false;
			return;
		}

		//Catch those who runs away
		if (dist > 2)
		{
			//position = Vector3(std::ceil(position.x), std::ceil(position.y), std::ceil(position.z));
			isPathFound = false;
			m_path.clear();
			return;
		}

		//std::cout << "Distance to:" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ": " << dist << std::endl;
		if (dist <= 0.1)
		{
			std::cout << "Reached: (" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ")" << std::endl;
			//If theres more nodes, set the next destination
			if (m_path_index + 1 < m_path.size())
			{
				std::cout << "Traveling towards: (" << m_path[m_path_index + 1].x << ", " << m_path[m_path_index + 1].y << ")" << std::endl;
				std::cout << "Current Pos: (" << position.x << ", " << position.y << ")" << std::endl;
				++m_path_index;
			}
			else
			{
				//std::cout << "Reached the Destination!" << std::endl;
				m_path.clear();
				isPathFound = false;
				return;
			}

			//std::cout << "Route: " << std::endl;
			for (int i = 0; i < m_path.size(); ++i)
			{
				if (i == 0)
					std::cout << "Start";
				std::cout << "->(" << m_path[i].x << ", " << m_path[i].y << ")";
				if (i == m_path.size() - 1)
					std::cout << "->End";
			}
			std::cout << std::endl;

		}
	}	

	//Check direction of next node
	Coord2D dir;
	if (m_path_index + 1 < m_path.size())
		dir = m_path[m_path_index] - m_path[m_path_index + 1];
	else
		dir = Coord2D((int)std::floor(position.x), (int)std::floor(position.y)) - m_path[m_path_index];

	if (dir == Coord2D(0, -1)) //Up
	{
		this->m_velocity += Vector3(0, 3, 0);
		return;
	}
	else if (dir == Coord2D(0, 1)) //Down
	{
		this->m_velocity += Vector3(0, -3, 0);
		return;
	}
	else if (dir == Coord2D(1, 0)) //Left
	{
		this->m_velocity += Vector3(-3, 0, 0);
		return;
	}
	else if (dir == Coord2D(-1, 0)) //Right
	{
		this->m_velocity += Vector3(3, 0, 0);
		return;
	}
}

void EnemyMelee::FindPath(Coord2D _src, Coord2D _end)
{
	m_result = std::async(std::launch::async, &PathFinder::findPath,
		&m_path_finder,
		Coord2D(_src),
		Coord2D(_end));
}
