#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../GenericEntity.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "../Items/Melee.h"
#include "../SpriteEntity.h"

#include <list>
#include <vector>
#include "../MapManager.h"
#include "../EntityManager.h"
#include "../CollisionManager.h"
#include "../ShortSword.h"
#include "../Bow.h"

Player::Player(void)
	: GenericEntity(MeshList::GetInstance()->GetMesh("player"))
	, m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_jump(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dElapsedTime(0.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, m_iLevel(0)
{
	type = GenericEntity::PLAYER_OBJ;
	tile_ID = 0;
	SetCollider(true);
}

Player::~Player(void)
{
	m_pTerrain = NULL;
}

// Initialise this class instance
void Player::Init(void)
{
	//Animate player
	SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("GEO_SPRITE_ANIMATION"));
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(1, 4, 1, 1.0f, true);
		animation = new SpriteEntity(sa);
		//EntityManager::GetInstance()->AddEntity(aSprite);
	}

	// Set the default values
	defaultPosition.Set(0,0,10);
	m_invincible = true;

	// Set the current values
	position.Set(3, 0, 0);
	m_defmov = 0.125f;
	m_combo = 0;
	m_attacking = false;
	m_isOnFloor = false;
	m_isClimbing = false;

	// Set Boundary
	maxBoundary.Set(.5, .5, .5);
	minBoundary.Set(-.5, -.5, -.5);
	velocity.Set(0, 0, 0);
	this->accleration.Set(0, 0, 0);
	direction.Set(0, 0, 0);
	last_direction = direction;

    m_interacted = false;
    m_interacttimer = 0;
    m_definteracttimer = 0.025f;

	SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
    this->m_player_equipment[EQUIPMENT_MELEE] = new ShortSword();
	this->m_player_equipment[EQUIPMENT_RANGED] = new Bow();

	for (int i = EQUIPMENT_MELEE;i < EQUIPMENT_FLASK;++i)
	{
		this->m_player_equipment[i]->Init(this);
	}
}

void Player::Update(double dt)
{
    if (m_interacted)
        this->m_interacttimer -= dt;
    if (m_interacted && m_interacttimer < 0)
        this->m_interacted = false;

    std::cout << m_interacted << std::endl;

	SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
	
	//Update sprite position
	animation->SetPosition(position);
	animation->SetScale(Vector3(0.5, 0.5, 0.5));
	animation->Update(dt);


	if (m_attacking)
		m_combotimer -= dt;
	else if (m_attacking && m_combotimer < 0)
	{
		this->m_combo = 0;
		this->m_combotimer = 0;
		this->m_attacking = false;
	}
	//std::cout << "<F>" << std::endl;
	//std::cout << "Position: " << position << std::endl;
	//std::cout << "Velocity: " << velocity << std::endl;
	//std::cout << "Accleration: " << accleration << std::endl;
	int temp_tile = MapManager::GetInstance()->GetLevel(m_iLevel)->ReturnTileViaPos(position);
	//std::cout << "X: " << position.x << " Y: " << position.y << " Z : " << position.z << std::endl;
	if (temp_tile != tile_ID)
	{
		tile_ID = temp_tile;
	}

	//std::cout << tile_ID << std::endl;

	this->UpdateJump(dt);
	this->UpdateMovment(dt);
    this->m_player_equipment[EQUIPMENT_MELEE]->Update(dt);
	this->attachedCamera->SetCameraPos(Vector3(position.x, position.y, 7));
	this->attachedCamera->SetCameraTarget(position);

	if (!m_isOnFloor && !m_isClimbing)
		accleration.y = -9.8;
	else if (m_isOnFloor && accleration != 0 && !m_isClimbing)
	{
		accleration.y = 0;
		velocity.y = 0;
	}

	if (m_isClimbing)
	{
		accleration.y = 0;
	}

	std::vector<EntityBase*> temp_blocks;
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, true);

		for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it < temp_blocks.end(); ++it)
		{

			if (!(*it)->HasCollider())
				continue;

			if (CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().y - 0.28f
				, 0), *it) || 
				CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x + 0.20f, Player::GetInstance()->GetPosition().y - 0.28f
					, 0), *it) ||
				CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x - 0.20f, Player::GetInstance()->GetPosition().y - 0.28f
					, 0), *it))
			{
				if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{
					if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::SOLID_BLOCK || dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM)
					{
						m_isOnFloor = true;				
						break;
					}
					else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::TOP_PLATFORM)
					{
						while (position.y - 0.26f < (*it)->GetPosition().y + (*it)->GetScale().y * 0.5)
						{
							position.y += 0.001;
						}

						m_isOnFloor = true;
						break;
					}
					else
					{
							m_isOnFloor = false;
					}
				}
			}
			else
			{
					m_isOnFloor = false;
			}
		}

		for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it < temp_blocks.end(); ++it)
		{
			if (!(*it)->HasCollider())
				continue;

			if (CollisionManager::GetInstance()->CheckAABBCollision(*it, Player::GetInstance()))
			{
				if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{
					//std::cout << dynamic_cast<TileEntity*>(*it)->block_type << std::endl;
					if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDER || dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM)
					{
						m_isClimbing = true;
						break;
					}
					else
					{
						m_isClimbing = false;
					}
				}
			}
			else
			{
				m_isClimbing = false;
			}
		}

		for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it < temp_blocks.end(); ++it)
		{
		
			if (!(*it)->HasCollider())
				continue;

			if (CollisionManager::GetInstance()->CheckAABBCollision(*it, Player::GetInstance()))
			{
				if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{

					//std::cout << dynamic_cast<TileEntity*>(*it)->block_type << std::endl;
					if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::SOLID_BLOCK)
					{
						if (!m_isClimbing)
						{
							double temp_pusher = 0;
							while (true)
							{
								if (!(CollisionManager::GetInstance()->CheckAABBCollision(*it, Player::GetInstance())))
								{
									break;
								}

								if (position.x > (*it)->GetPosition().x)
								{
									temp_pusher = 0.001;
								}
								else
								{
									temp_pusher = -0.001;
								}

								velocity.x = 0;

								position.x += temp_pusher;
								SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));

							}
						}
						else
						{
							if(!m_isClimbing)
								SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));

							break;
						}
					}
				/*	else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::TOP_PLATFORM)
					{
						if (CollisionManager::GetInstance()->CheckPlayerDirCollision(*it))
						{
							if(!m_isOnFloor)
							velocity.y = 0;
							SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
							break;
						}
					}*/
				}
			}
		}

	std::cout << "CLIMBING: " << m_isClimbing << std::endl;
	std::cout << "ON FLOOR: " << m_isOnFloor << std::endl;

}

void Player::UpdateMovment(double dt)
{
	if (m_moving && m_movingtimer > 0)
		m_movingtimer -= dt;
	else
	{
		m_moving = false;
		m_movingtimer = m_defmov;
	}
	this->velocity += this->accleration * dt;
	//Math::Clamp(std::abs(velocity.x), 0.f, 10.f);
	Vector3 temp = this->position + velocity * dt;

	std::vector<EntityBase*> temp_blocks;
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, true);

	SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (temp.y + (maxBoundary.y * 0.5)), (temp.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (temp.y + (minBoundary.y * 0.5)), (temp.z + (minBoundary.z * 0.5))));

	for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it <= temp_blocks.end(); ++it)
	{
		if (it == temp_blocks.end())
		{
			this->position = temp;
			break;
		}

		if (!(*it)->HasCollider())
			continue;

		if (CollisionManager::GetInstance()->CheckAABBCollision(*it, Player::GetInstance()))
		{
			if (dynamic_cast<TileEntity*>(*it) != nullptr)
			{

				//std::cout << dynamic_cast<TileEntity*>(*it)->block_type << std::endl;
				if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::SOLID_BLOCK)
				{		
					if (!m_isClimbing)
					{
						SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
						velocity.y = 0;
					}
					else
						this->position = temp;
					break;
				}
				else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::TOP_PLATFORM)
				{

					if (CollisionManager::GetInstance()->CheckPlayerDirCollision(*it))
					{
						if (position.y - 0.28f > (*it)->GetPosition().y + (*it)->GetScale().y * 0.5)
						{
							velocity.y = 0;
							SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
						}
						else
							this->position = temp;
						break;
					}
					else
					{
						this->position = temp;
						break;
					}

				}
				else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM)
				{
					if (!m_isClimbing)
					{
						if (CollisionManager::GetInstance()->CheckPlayerDirCollision(*it))
						{
							velocity.y = 0;
							SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
							break;
						}
						else
						{
							if (temp.y < MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfLevel() * MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfTileSet())
								this->position = temp;
							else
								this->position.x = temp.x;
							break;
						}
					}
					else
					{
						if (temp.y < (MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfLevel() * MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfTileSet()) - 1)
							this->position = temp;
						else
							this->position.x = temp.x;
						break;
					}
				}
			}
		}
	}


	if (!m_moving)
	{
		last_position = position;
		if (abs(velocity.x) > 0)
		{
			velocity.x -= (velocity.x * 5) * dt;
			if (velocity.x > -0.01f && velocity.x < 0.01f)
				velocity.x = 0;
			else
				velocity.x = direction.x * Math::Clamp(std::abs(velocity.x), Math::EPSILON, 10.0f);
		}
	}
	this->accleration.x = 0;
}

void Player::UpdateJump(double dt)
{
	if (m_jump)
	{
		if (velocity.y < 0)
		{
			velocity.y = 0;
			direction.y = -1;
			m_jump = false;
		}
	}
}

void Player::Render()
{
    //Collision::Render();

	this->animation->Render();

   /* this->m_player_equipment[EQUIPMENT_MELEE]->Render();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();*/
}


void Player::MoveUp(double dt)
{
	if (m_isClimbing)
	{
		last_direction.y = direction.y;
		this->direction.y = 1;
		this->velocity.y = 1;
		this->accleration.y = 3.8;
		m_movingtimer = m_defmov;
		m_movingtimer = 0.125f;

		this->m_moving = true;
	}
}

void Player::MoveDown(double dt)
{
	std::vector<EntityBase*> temp_blocks;
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, true);

	for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it < temp_blocks.end(); ++it)
	{

		if (!(*it)->HasCollider())
			continue;

		if (CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().y - 0.32f
			, 0), *it))
		{
			if (dynamic_cast<TileEntity*>(*it) != nullptr)
			{
				if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM || dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDER)
				{
					m_isClimbing = true;
					break;
				}
			}
		}
		else
		{
			m_isClimbing = false;
		}
	}

	if (m_isClimbing)
	{
		last_direction.y = direction.y;
		this->direction.y = -1;
		this->velocity.y = -1;
		this->accleration.y = -3.8;
		m_movingtimer = m_defmov;
		m_movingtimer = 0.125f;

		this->m_moving = true;
	}
}

void Player::MoveRight(double dt)
{
	last_direction.x = direction.x;
	this->direction.x = 1;
	this->accleration.x = 20;
	this->m_moving = true;
	m_movingtimer = m_defmov;
}

void Player::MoveLeft(double dt)
{
	last_direction.x = direction.x;
	this->direction.x = -1;
	this->accleration.x = -20;
	m_movingtimer = m_defmov;
	this->m_moving = true;
}

void Player::Jump(double dt)
{
	last_direction.y = direction.y;
	this->direction.y = 1;
	this->m_jump = true;
	this->m_isOnFloor = false;
	this->velocity.y = 2.5;
	this->accleration.y = -9.8f;
}

void Player::DodgeRoll(double dt)
{
	m_invincible = true;
	this->velocity.x = 20 * this->direction.x;
	this->m_moving = true;
	m_movingtimer = 0.125;
}

void Player::PrimaryAttack(double dt, int _combo)
{
	Melee* Weapon = dynamic_cast<Melee*>(this->m_player_equipment[EQUIPMENT_MELEE]);
	if (Weapon)
	{
		Weapon->Action(_combo + m_combo);
		this->m_attacking = true;
		this->m_combotimer = 0.15f;
		++this->m_combo;
		if (m_combo >= 3)
			m_combo = 0;
	}
}

void Player::SecondaryAttack(double dt,int _actiontype)
{
	Ranged* Weapon = dynamic_cast<Ranged*>(this->m_player_equipment[EQUIPMENT_RANGED]);

	if (Weapon)
	{
		Weapon->Action(_actiontype);
	}
}

void Player::Interact(double dt)
{
    this->m_interacted = true;
    this->m_interacttimer = this->m_definteracttimer;

    //Add Interact Codes Here.
}

void Player::TakeDamage(int _dmg)
{
	this->m_health -= _dmg;
}

int Player::GetHealth()
{
	return this->m_health;
}

FPSCamera * Player::getCamera()
{
	return attachedCamera;
}

void Player::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
	Vector3 target = position;
	Vector3 up = Vector3(0, 1, 0);
	attachedCamera->Init(Vector3(position.x, position.y, 10), target, up);
	std::cout << up << std::endl;
}

void Player::DetachCamera()
{
	attachedCamera = nullptr;
}

int Player::GetCurrentLevel()
{
	return m_iLevel;
}

Vector3 Player::GetDirection()
{
	return this->direction;
}

Vector3 Player::GetPlayerVelocity()
{
	return velocity;
}