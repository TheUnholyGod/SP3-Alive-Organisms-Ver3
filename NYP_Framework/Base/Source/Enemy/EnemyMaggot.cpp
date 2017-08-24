#include "EnemyMaggot.h"

#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"

EnemyMaggot::EnemyMaggot(Mesh * mesh,
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


	m_state = AI_STATES::AI_CHASE;
	dir = true;
}

EnemyMaggot::~EnemyMaggot()
{
}

void EnemyMaggot::Update(double _dt)
{
	//Create AABB for collision
	this->GenerateAABB(this->position);

	//Update enemy pos
	this->position += m_velocity * _dt;

	this->animation->SetPosition(position);
	this->animation->Update(_dt);

	this->animation2->SetPosition(position);
	this->animation2->Update(_dt);

	Detect(_dt);

	switch (m_state)
	{
	case EnemyMaggot::AI_CHASE:
	{
		Move();
		//std::cout << "Chasing" << std::endl;
		break;
	}
	case EnemyMaggot::AI_ATTACK:
	{
		Attack();
		//std::cout << "Attack" << std::endl;
		break;
	}
	default:
		break;
	}

	//Update tileID for spatial partition
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position);
}

void EnemyMaggot::Render()
{
	if (m_state == AI_ATTACK)
		this->animation2->Render();
	else
		this->animation->Render();
}

bool EnemyMaggot::CollisionResponse(GenericEntity *ThatEntity)
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

void EnemyMaggot::Move()
{
	m_velocity.SetZero();

	if (dir)
	{
		m_velocity = Vector3(1, 0, 0);
		this->animation->SetRotation(180, Vector3(0, 1, 0));
	}
	else
	{
		m_velocity = Vector3(-1, 0, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
	}
	//while (dir) //right
	//{
	//	if (!m_path_finder.detectCollision(Coord2D((int)(position.x) + 1, std::floor(position.y + 0.5))))
	//	{
	//		//std::cout << "Right no collision" << std::endl;
	//		m_velocity = Vector3(1, 0, 0);
	//		this->animation->SetRotation(180, Vector3(0, 1, 0));
	//		break;
	//	}
	//	else
	//	{
	//		//dir = !dir;
	//		break;
	//	}
	//}
	//while (!dir) //left
	//{
	//	if (!m_path_finder.detectCollision(Coord2D((int)(position.x), std::floor(position.y + 0.5))))
	//	{
	//		//std::cout << "Left no collision" << std::endl;
	//		m_velocity = Vector3(-1, 0, 0);
	//		this->animation->SetRotation(0, Vector3(0, 1, 0));
	//		break;
	//	}
	//	else
	//	{
	//		//dir = !dir;
	//		break;
	//	}
	//}
}

void EnemyMaggot::Detect(double dt)
{
	//Find if player on the left or on the right
	//Set direction accordingly

	float distX = Player::GetInstance()->GetPosition().x - position.x; //Find which side the player is on
	float dist = (Player::GetInstance()->GetPosition() - position).Length(); //Find how far away the player is

	if (distX > 0) //player on the right
	{
		dir = true;
		//std::cout << "Player on the right" << std::endl;
	}
	else
	{
		dir = false;
		//std::cout << "Player on the left" << std::endl;
	}

	if (dist < 0.1)
		m_state = AI_ATTACK;
	else
		m_state = AI_CHASE;
	
}

void EnemyMaggot::Attack()
{
	m_velocity.SetZero();
	//Do damage to player
	std::cout << "Deal dmg to player" << std::endl;

	//Go back to chase
	m_state = AI_CHASE;
	return;
}