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
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position, Player::GetInstance()->GetIsFightingBoss()));
	this->SetCollider(have_collider);
	this->SetPhysic(have_physic);
	this->SetStatic(false);
	this->SetActive(false);

	m_state = AI_STATES::AI_CHASE;
	this->type = GenericEntity::PLAGUE_MAGGOT_OBJ;
	dir = true;
}

EnemyMaggot::~EnemyMaggot()
{
}

void EnemyMaggot::Init(Vector3 pos)
{
	this->type = GenericEntity::PLAGUE_MAGGOT_OBJ;

	this->position = pos;
	this->SetActive(true);
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
}

void EnemyMaggot::Update(double _dt)
{
	if (!(this->m_active))
		return;

	//Create AABB for collision
	this->GenerateAABB(this->position);

	//Update enemy pos
	this->position += m_velocity * _dt;

	this->animation->SetPosition(position);
	this->animation->Update(_dt);

	this->animation2->SetPosition(position);
	this->animation2->Update(_dt);

	this->animation->SetScale(scale);
	this->animation2->SetScale(scale);

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
	this->tile_ID = 0;//MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position);
}

void EnemyMaggot::Render()
{
	if (!(this->m_active))
		return;

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
		this->animation->SetRotation(0, Vector3(0, 1, 0));
	}
	else
	{
		m_velocity = Vector3(-1, 0, 0);
		this->animation->SetRotation(180, Vector3(0, 1, 0));
	}
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
	std::cout << "Deal dmg to player 100 " << std::endl;

	Player::GetInstance()->TakeDamage(100);
	std::cout << "player hp: "<< Player::GetInstance()->GetHealth() << std::endl;
	//Go back to chase
	m_state = AI_CHASE;
	return;
}