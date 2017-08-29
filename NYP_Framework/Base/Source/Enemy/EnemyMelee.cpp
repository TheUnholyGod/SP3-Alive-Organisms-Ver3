#include "EnemyMelee.h"

#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"
#include "../CollisionManager.h"
#include "../SpriteEntity.h"
#include "../Particle Effect/ParticleEffect.h"

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
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position, Player::GetInstance()->GetIsFightingBoss()));
	this->SetCollider(have_collider);
	this->SetPhysic(have_physic);
	this->SetStatic(false);
	this->m_health = 30;
	this->GenerateAABB(this->position);
	this->type = GenericEntity::ENEMY_OBJ;
	m_state = AI_STATES::AI_PATROL;
	dir = true;
}

EnemyMelee::~EnemyMelee()
{
	if (animation)
	{
		delete animation;
		animation = nullptr;
	}
	if (animation2)
	{
		delete animation2;
		animation2 = nullptr;
	}
	this->m_result.~future();
}

void EnemyMelee::Update(double _dt)
{
	if (this->m_health <= 0)
	{
		Create::Particle("particle",
			position,
			Vector3(0, 0, 0),
			EFFECT_TYPE::EFT_HIT, 0.3, 0.3,
			Player::GetInstance()->GetIsFightingBoss());

		this->SetIsDone(true);
	}

	//Create AABB for collision
	this->GenerateAABB(this->position);

	//Update enemy pos
	this->position += m_velocity * _dt;

	this->animation->SetPosition(position);
	this->animation->Update(_dt);

	this->animation2->SetPosition(position);
	this->animation2->Update(_dt);
	
	if (m_stunned_duration > 0)
	{
		m_stunned_duration -= _dt;
		return;
	}

	if (m_state == AI_ATTACKING)
	{
		m_attackCooldown -= _dt;
	}

	Detect(_dt);

	switch (m_state)
	{
	case EnemyMelee::AI_PATROL:
	{
		Patrol();
		//std::cout << "Patrolling" << std::endl;
		break;
	}
	case EnemyMelee::AI_CHASE:
	{
		Move();
		//std::cout << "Chasing" << std::endl;
		break;
	}
	case EnemyMelee::AI_ATTACK:
	{
		m_attackCooldown = 1.0;
		m_state = AI_ATTACKING;
		//std::cout << "Attack" << std::endl;
		break;
	}
	case EnemyMelee::AI_ATTACKING:
	{
		Attack();
		break;
	}
	case EnemyMelee::AI_RETURN:
	{
		break;
	}	
	case EnemyMelee::AI_STUNNED:
	{
		m_stunned_duration = 2;
		break;
	}
	default:
		break;
	}

	//Iterate through the path vector and move through the terrain
	/*Move();
	if(m_path.empty())
		Patrol();*/
	
	//Update tileID for spatial partition
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position, Player::GetInstance()->GetIsFightingBoss());
}

void EnemyMelee::Render()
{
	if (m_state == AI_ATTACKING)
		this->animation2->Render();
	else
		this->animation->Render();
}

bool EnemyMelee::CollisionResponse(GenericEntity *ThatEntity)
{
	/*Collision response for this enemy*/
	if (ThatEntity->type == PROJECTILE_OBJ || ThatEntity->type == HITBOX_OBJ)
	{
		std::cout << "Dieded" << std::endl;
		ThatEntity->SetActive(false);
		this->SetIsDone(true);
	}
	return false;
}

void EnemyMelee::Move()
{
	m_velocity.SetZero();

	//Dont move when path nodes are empty
	if (m_path.empty())
	{
		isPathFound = false;
		m_state = AI_PATROL;
		return;
	}

	//Check if enemy is already at node
	float dist = 999;
	if (m_path_index + 1 < m_path.size()) //Make sure its not the end of the path
	{
		dist = (position - Vector3(m_path[m_path_index + 1].x, m_path[m_path_index + 1].y, 0)).Length();
	}
	else //At the end of the path
	{
		//std::cout << "Reached the Destination! & cleared path!" << std::endl;
		m_state = AI_PATROL;
		m_path.clear();
		isPathFound = false;
		return;
	}
	

	//Catch those who runs away
	if (dist > 2)
	{
		//position = Vector3(std::ceil(position.x), std::ceil(position.y), std::ceil(position.z));
		m_velocity.SetZero();
		
		isPathFound = false;
		m_path.clear();
		//m_state = AI_PATROL;
		return;
	}

	//std::cout << "Distance to:" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ": " << dist << std::endl;
	if (dist <= 0.1)
	{
		//std::cout << "Reached: (" << m_path[m_path_index].x << ", " << m_path[m_path_index].y << ")" << std::endl;
		//If theres more nodes, set the next destination
		if (m_path_index + 1 < m_path.size())
		{
			//std::cout << "Traveling towards: (" << m_path[m_path_index + 1].x << ", " << m_path[m_path_index + 1].y << ")" << std::endl;
			//std::cout << "Current Pos: (" << position.x << ", " << position.y << ")" << std::endl;
			++m_path_index;
		}
		else
		{
			//std::cout << "Reached the Destination!" << std::endl;
			m_path.clear();
			isPathFound = false;
			return;
		}
	}


	//Check direction of next node
	Coord2D dir;
	if (m_path_index + 1 < m_path.size())
		dir = m_path[m_path_index] - m_path[m_path_index + 1];
	else
		dir = Coord2D((int)std::floor(position.x), (int)std::floor(position.y)) - m_path[m_path_index];

	float speed = 3.0f;
	if (dir == Coord2D(0, -1)) //Up
	{
		this->m_velocity += Vector3(0, speed, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
		this->animation2->SetRotation(0, Vector3(0, 1, 0));
		return;
	}
	else if (dir == Coord2D(0, 1)) //Down
	{
		this->m_velocity += Vector3(0, -speed, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
		this->animation2->SetRotation(0, Vector3(0, 1, 0));
		return;
	}
	else if (dir == Coord2D(1, 0)) //Left
	{
		this->m_velocity += Vector3(-speed, 0, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
		this->animation2->SetRotation(0, Vector3(0, 1, 0));
		return;
	}
	else if (dir == Coord2D(-1, 0)) //Right
	{
		this->m_velocity += Vector3(speed, 0, 0);
		this->animation->SetRotation(180, Vector3(0, 1, 0));
		this->animation2->SetRotation(180, Vector3(0, 1, 0));
		return;
	}
}

void EnemyMelee::Patrol()
{
	m_velocity.SetZero();

	/*std::cout << "Pos: (" << position.x << ", " << position.y << ")" << std::endl;
	std::cout << "Corrected(R): (" << (int)(position.x) + 1 << ", " << (int)(position.y) << std::endl;
	std::cout << "Corrected(L): (" << (int)(position.x) << ", " << (int)(position.y) << std::endl;*/
	
	//position.y = std::ceil(position.y);
	while (dir) //right
	{
		
		if (!m_path_finder.detectCollision(Coord2D((int)(position.x) + 1, std::floor(position.y + 0.5))))
		{
			//std::cout << "Right no collision" << std::endl;
			m_velocity = Vector3(1, 0, 0);
			this->animation->SetRotation(180, Vector3(0, 1, 0));
			break;
		}
		else
		{
			dir = !dir;
			break;
		}
	}
	while (!dir) //left
	{
		if (!m_path_finder.detectCollision(Coord2D((int)(position.x), std::floor(position.y + 0.5))) && position.x > 0)
		{
			//std::cout << "Left no collision" << std::endl;
			m_velocity = Vector3(-1, 0, 0);
			this->animation->SetRotation(0, Vector3(0, 1, 0));
			break;
		}
		else
		{
			dir = !dir;
			break;
		}
	}
}

void EnemyMelee::Detect(double dt)
{
	float dist = (Player::GetInstance()->GetPosition() - position).Length();

	if (m_state == AI_ATTACKING) return;

	if (dist < 0.5)
	{
		//isPathFound = false;
		//m_path.clear();
		m_state = AI_ATTACK;
	}
	else if (dist > 10)
	{
		isPathFound = false;
		m_path.clear();
		m_state = AI_PATROL;
	}
	else
	{
		//Increment the time since last update for regulated checks
		m_timeSinceLastUpdate += dt;

		//Player pos to find the player
		Vector3 playerpos = Player::GetInstance()->GetPosition();

		//Find the path every 5 sec
		if (m_timeSinceLastUpdate > 1 && !isPathFound && m_path.empty())
		{
			//std::cout << "Calling pathfinder!" << std::endl;
			FindPath({ (int)(position.x + 0.5), (int)(position.y + 0.5) },
			{ (int)std::floor(playerpos.x + 0.5), (int)std::floor(playerpos.y + 0.5) });
			m_timeSinceLastUpdate = 0;
		}
		//Check if worker thread is done, if done, obtain result & change state
		if (m_result.valid() && !isPathFound)
		{
			if (m_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				m_path.clear();
				m_path = m_result.get();
				m_path_index = 0;
				if (!m_path.empty())
				{
					m_state = AI_CHASE;
					isPathFound = true;

					//Print the route out for debug
					//std::cout << "Route: " << std::endl;
					/*for (int i = 0; i < m_path.size(); ++i)
					{
						if (i == 0)
							std::cout << "Start";
						std::cout << "->(" << m_path[i].x << ", " << m_path[i].y << ")";
						if (i == m_path.size() - 1)
							std::cout << "->End";
					}
					std::cout << std::endl;*/
				}
				else
				{
					m_state = AI_PATROL;
				}
				
			}
		}
	}
}

void EnemyMelee::Attack()
{
	m_velocity.SetZero();
	//Do damage to player
	if (m_attackCooldown <= 0)
	{
		if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
		{
			std::cout << "Dealt 20 damage to player" << std::endl;
			Player::GetInstance()->TakeDamage(10);
			Create::Particle("particle", 
				Player::GetInstance()->GetPosition(), 
				Vector3(0, 0, 0), 
				EFFECT_TYPE::EFT_HIT, 0.3, 0.3, 
				Player::GetInstance()->GetIsFightingBoss());
			
		}
		m_state = AI_CHASE;
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
