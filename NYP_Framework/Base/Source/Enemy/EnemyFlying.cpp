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
#include "../Particle Effect/ParticleEffect.h"
#include "../CollisionManager.h"

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
	this->m_health = 10;
	this->GenerateAABB(this->position);
	this->type = GenericEntity::ENEMY_OBJ;

	m_state = AI_STATES::AI_IDLE;
	dir = true;
}

EnemyFlying::~EnemyFlying()
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

void EnemyFlying::Update(double _dt)
{
	//Despawn when 0 hp
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

	Detect(_dt);
	
	if (m_state == AI_ATTACKING)
	{
		m_attackCooldown -= _dt;
		//std::cout << "Attack Cooldown: " << m_attackCooldown << std::endl;
	}

	switch (m_state)
	{
	case EnemyFlying::AI_CHASE:
	{
		Move();
		break;
	}
	case EnemyFlying::AI_ATTACK:
	{
		m_attackCooldown = 0.8;
		m_state = AI_ATTACKING;
		break;
	}	
	case EnemyFlying::AI_ATTACKING:
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
	if(m_state == AI_ATTACKING)
		this->animation2->Render();
	else
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
		this->animation->SetRotation(180, Vector3(0, 1, 0));
	}
	if (position.x < playerPos.x)
	{
		m_velocity += Vector3(1, 0, 0);
		this->animation->SetRotation(0, Vector3(0, 1, 0));
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

	if (m_state == AI_ATTACKING) return;

	if (dist < 0.3)
	{
		m_state = AI_ATTACK;
		return;
	}
	else if (dist > 3)
	{
		m_state = AI_IDLE;
		m_velocity.SetZero();
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
	if (m_attackCooldown <= 0)
	{
		if (CollisionManager::GetInstance()->CheckAABBCollision(this, Player::GetInstance()))
		{
			std::cout << "Dealt 20 damage to player" << std::endl;
			Player::GetInstance()->TakeDamage(50);

			Create::Particle("particle",
				Player::GetInstance()->GetPosition(),
				Vector3(0, 0, 0),
				EFFECT_TYPE::EFT_HIT, 0.3, 0.3,
				Player::GetInstance()->GetIsFightingBoss());
		}
		Create::Particle("particle",
			position,
			Vector3(0, 0, 0),
			EFFECT_TYPE::EFT_HIT, 0.3, 0.3,
			Player::GetInstance()->GetIsFightingBoss());

		this->SetIsDone(true);

	}
}
