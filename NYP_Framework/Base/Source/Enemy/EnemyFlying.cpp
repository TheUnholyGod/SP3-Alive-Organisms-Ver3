#include "EnemyFlying.h"

#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"

EnemyFlying::EnemyFlying(Mesh * mesh,
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


	m_state = AI_STATES::AI_IDLE;
	dir = true;
}

EnemyFlying::~EnemyFlying()
{
}

void EnemyFlying::Update(double _dt)
{
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

	Detect(_dt);

	switch (m_state)
	{
	case EnemyFlying::AI_CHASE:
	{
		Move();
		break;
	}
	case EnemyFlying::AI_ATTACK:
	{
		Attack();
		break;
	}
	case EnemyFlying::AI_STUNNED:
	{
		m_stunned_duration = 2;
		break;
	}
	default:
		break;
	}


	//Update tileID for spatial partition
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position, Player::GetInstance()->GetIsFightingBoss());
}

void EnemyFlying::Render()
{
	this->animation->Render();
}

bool EnemyFlying::CollisionResponse(GenericEntity *ThatEntity)
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

void EnemyFlying::Move()
{
	m_velocity.SetZero();
	Vector3 playerPos = Player::GetInstance()->GetPosition();

	if (position.x > playerPos.x)
	{
		m_velocity += Vector3(-1, 0, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
	}
	if (position.x < playerPos.x)
	{
		m_velocity += Vector3(1, 0, 0);
		this->animation->SetRotation(180, Vector3(0, 1, 0));
	}
	if (position.y > playerPos.y)
	{
		m_velocity += Vector3(0, -1, 0);
	}
	if (position.y < playerPos.y)
	{
		m_velocity += Vector3(0, 1, 0);
	}
}


void EnemyFlying::Detect(double dt)
{
	float dist = (Player::GetInstance()->GetPosition() - position).Length();

	if (dist < 0.8)
	{
		m_state = AI_ATTACK;
	}
	else if (dist > 10)
	{
		m_state = AI_IDLE;
	}
	else
	{
		m_state = AI_CHASE;
	}
}

void EnemyFlying::Attack()
{
	m_velocity.SetZero();
	//Do damage to player
	std::cout << "Deal dmg to player" << std::endl;

	//Go back to chase
	m_state = AI_CHASE;
	return;
}
