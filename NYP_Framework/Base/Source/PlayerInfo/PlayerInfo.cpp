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
#include "../Manager/GameStateManager.h"
#include "../FamineBoss.h"

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
	this->m_health = 100;
	m_maxHealth = m_health;
	m_regenTimer = 0;
}

Player::~Player(void)
{
	m_pTerrain = NULL;
	delete animation;
	delete animationWalking;
	delete animationWalkingLeft;
	delete animationClimbing;
	delete animationJumping;
	delete attachedCamera;
	
	for (int i = 0; i < 2; ++i)
	{
		delete m_player_equipment[i];
	}
}

// Initialise this class instance
void Player::Init(void)
{
	//Animate player
	SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("Idle_Animation"));
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 2, 1, 1.0f, true);
		animation = new SpriteEntity(sa); //Create new entity for the animiation
		//EntityManager::GetInstance()->AddEntity(aSprite);
	}

	SpriteAnimation* sawalking = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("Running_Animation"));
	if (sawalking)
	{
		sawalking->m_anim = new Animation();
		sawalking->m_anim->Set(0, 2, 1, 1.0f, true);
		animationWalking = new SpriteEntity(sawalking); //Create new entity for the animiation
		//EntityManager::GetInstance()->AddEntity(aSprite);
	}

	SpriteAnimation* sawalkingleft = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("Running_Animationleft"));
	if (sawalkingleft)
	{
		sawalkingleft->m_anim = new Animation();
		sawalkingleft->m_anim->Set(0, 2, 1, 1.0f, true);
		animationWalkingLeft = new SpriteEntity(sawalkingleft); //Create new entity for the animiation
		//EntityManager::GetInstance()->AddEntity(aSprite);
	}

	SpriteAnimation* sajumping = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("Jumping_Animation"));
	if (sajumping)
	{
		sajumping->m_anim = new Animation();
		sajumping->m_anim->Set(0, 1, 1, 1.0f, true);
		animationJumping= new SpriteEntity(sajumping); //Create new entity for the animiation
       //EntityManager::GetInstance()->AddEntity(aSprite);
	}

	SpriteAnimation* saclimbing = dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("Climbing_Animation"));
	if (saclimbing)
	{
		saclimbing->m_anim = new Animation();
		saclimbing->m_anim->Set(0, 1, 1, 1.0f, true);
		animationClimbing = new SpriteEntity(saclimbing); //Create new entity for the animiation
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
	m_isFightingBoss = false;
	m_isKilledBoss = false;
	m_canDoubleJump = false;

	// Set Boundary
	maxBoundary.Set(.5, .5, .5);
	minBoundary.Set(-.5, -.5, -.5);
	velocity.Set(0, 0, 0);
	this->accleration.Set(0, 0, 0);
	direction.Set(0, 0, 0);
	last_direction = direction;

    m_interacted = false;
    m_interacttimer = 0;
    m_definteracttimer = 1.f;
    m_invincibletimer = 0;
    m_definvincibletimer = 0.5;
	maxofthemaxvelocity.Set(20, 20, 0);
	maxvelocity.Set(10, 10, 0);

	SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
    this->m_player_equipment[EQUIPMENT_MELEE] = new ShortSword();
	this->m_player_equipment[EQUIPMENT_RANGED] = new Bow();

	for (int i = EQUIPMENT_MELEE;i < EQUIPMENT_FLASK;++i)
	{
		this->m_player_equipment[i]->Init(this);
	}
	this->m_dmg = 100;
}

void Player::Update(double dt)
{
	m_regenTimer += dt;

	if (this->m_health <= 0)
	{
		GameStateManager::GetInstance()->setState(GS_GAMEOVER);
	}

	if (m_health < 0)
	{
		//DO GAME OVER HERE
		m_health = 0;
	}

    if (m_interacted)
        this->m_interacttimer -= dt;
    if (m_interacted && m_interacttimer < 0)
        this->m_interacted = false;

    if (m_invincible)
    {
        //std::cout <<"i"<< m_invincibletimer << std::endl;
        this->m_invincibletimer -= dt;
    }
    if (m_invincible && m_invincibletimer < 0)
    {
        //std::cout << "no invis" << std::endl;
        this->m_invincible = false;
    }

	int health_up = 0, attack_up = 0, speed_up = 0;
	for (int i = 0; i < 2; ++i)
	{
		std::vector<Runes*> temp_rune_vector = m_player_equipment[i]->getRunes();
		if (temp_rune_vector.size() > 0)
		{
			for (std::vector<Runes*>::iterator it = temp_rune_vector.begin(); it != temp_rune_vector.end(); ++it)
			{
				std::vector<Attribute*> temp_attribute_vector = (*it)->getAttribute();
				for (std::vector<Attribute*>::iterator it2 = temp_attribute_vector.begin(); it2 != temp_attribute_vector.end(); ++it2)
				{
					switch ((*it2)->m_element)
					{
					case ET_FIRE:
						++attack_up;
						break;
					case ET_EARTH:
					case ET_WATER:
						++health_up;
						break;
					case ET_AIR:
					case ET_NONE:
						++speed_up;
						break;
					default:
						break;
					}
				}
			}
		}
	}

	if (health_up > 0 && m_maxHealth != (100 + (health_up * 10)))
	{
		m_maxHealth = 100 + (health_up * 10);
	}

	if (attack_up > 0)
	{
		this->m_dmg += 5 + (attack_up * 2);
	}

	if (speed_up > 0)
	{
		this->maxvelocity.x = Math::Min(velocity.x + 40 + (speed_up * 5), maxofthemaxvelocity.x);
	}

	if (m_health < m_maxHealth && m_regenTimer > 1.7f)
	{
		m_health += 2;
		m_regenTimer = 0;

		if (m_health > m_maxHealth)
			m_health = m_maxHealth;
	}

    //std::cout << m_health << std::endl;

	SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
	
	//Update sprite position
	animation->SetPosition(Vector3(position.x, position.y - 0.05, 0.2));
	animation->SetScale(Vector3(0.42, 0.45, 0.5));
	animation->Update(dt);

	animationWalking->SetPosition(Vector3(position.x, position.y - 0.05, 0.2));
	animationWalking->SetScale(Vector3(0.5, 0.5, 0.5));
	animationWalking->Update(dt);

	animationWalkingLeft->SetPosition(Vector3(position.x, position.y - 0.05, 0.2));
	animationWalkingLeft->SetScale(Vector3(0.5, 0.5, 0.5));
	animationWalkingLeft->Update(dt);

	animationJumping->SetPosition(Vector3(position.x, position.y - 0.05, 0.2));
	animationJumping->SetScale(Vector3(0.5, 0.5, 0.5));
	animationJumping->Update(dt);

	animationClimbing->SetPosition(Vector3(position.x, position.y - 0.05, 0.2));
	animationClimbing->SetScale(Vector3(0.5, 0.5, 0.5));
	animationClimbing->Update(dt);


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
	int temp_tile = MapManager::GetInstance()->GetLevel(m_iLevel)->ReturnTileViaPos(position, Player::GetInstance()->GetIsFightingBoss());;

	//std::cout << "X: " << position.x << " Y: " << position.y << " Z : " << position.z << std::endl;
	if (temp_tile != tile_ID)
	{
		tile_ID = temp_tile;
	}

	//std::cout << tile_ID << std::endl;

	this->UpdateJump(dt);
	this->UpdateMovement(dt);
    this->m_player_equipment[EQUIPMENT_MELEE]->Update(dt);

	if (!m_isKilledBoss)
	{
		this->attachedCamera->SetCameraPos(Vector3(position.x, position.y, 7));
		this->attachedCamera->SetCameraTarget(position);
	}
	else
	{
		this->attachedCamera->SetCameraPos(Vector3(position.x + Math::RandFloatMinMax(-.1, .1), position.y + Math::RandFloatMinMax(-.1, .1), 7));
		this->attachedCamera->SetCameraTarget(Vector3(position.x + Math::RandFloatMinMax(-.1, .1), position.y + Math::RandFloatMinMax(-.1, .1), 0));
	}

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
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, Player::GetInstance()->GetIsFightingBoss());

		for (std::vector<EntityBase*>::iterator it = temp_blocks.begin(); it < temp_blocks.end(); ++it)
		{

			if (!(*it)->HasCollider())
				continue;

			if (CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().y - 0.28f
				, 0), *it) || 
				CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x + 0.25f, Player::GetInstance()->GetPosition().y - 0.28f
					, 0), *it) ||
				CollisionManager::GetInstance()->CheckPointToAABBCollision(Vector3(Player::GetInstance()->GetPosition().x - 0.25f, Player::GetInstance()->GetPosition().y - 0.28f
					, 0), *it))
			{
				if (dynamic_cast<TileEntity*>(*it) != nullptr)
				{
					if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::SOLID_BLOCK || dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM)
					{
						m_canDoubleJump = false;
						m_isOnFloor = true;				
						break;
					}
					else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::TOP_PLATFORM)
					{
						while (position.y - 0.26f < (*it)->GetPosition().y + (*it)->GetScale().y * 0.5)
						{
							position.y += 0.001;
						}
						m_canDoubleJump = false;
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
							velocity.y = 0;
							position.y += 0.01;
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

	//std::cout << "CLIMBING: " << m_isClimbing << std::endl;
	//std::cout << "ON FLOOR: " << m_isOnFloor << std::endl;
}

void Player::UpdateMovement(double dt)
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
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, Player::GetInstance()->GetIsFightingBoss());

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
					{
						this->position = temp;
					}
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
				else if (dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDERWITHPLATFORM || dynamic_cast<TileEntity*>(*it)->block_type == TileEntity::LADDER)
				{
					/*if (!m_isClimbing)
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
					}*/
					if (m_isClimbing)
					{
						bool move_X = false, move_Y = false;
						if (temp.y < ((MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfLevel() * MapManager::GetInstance()->GetLevel(m_iLevel)->GetSizeOfTileSet()) - 1) + ((GetIsFightingBoss()) ? 100 : 0))
						{
							SetAABB(Vector3((temp.x + (maxBoundary.x * 0.5)) + 0.05, (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((temp.x + (minBoundary.x * 0.5)) - 0.05, (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
							for (std::vector<EntityBase*>::iterator it2 = temp_blocks.begin(); it2 <= temp_blocks.end(); ++it2)
							{
								if (it2 == temp_blocks.end())
								{
									move_X = true;
									break;
								}

								if (!(*it2)->HasCollider())
									continue;
								if (it == it2)
									continue;

								if (dynamic_cast<TileEntity*>(*it2)->block_type == TileEntity::SOLID_BLOCK)
								{
									if (CollisionManager::GetInstance()->CheckAABBCollision(*it2, Player::GetInstance()))
									{
										move_X = false;
										break;
									}
								}
							}

							SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (temp.y + (maxBoundary.y * 0.5)) + 0.02, (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (temp.y + (minBoundary.y * 0.5)) - 0.02, (position.z + (minBoundary.z * 0.5))));
							for (std::vector<EntityBase*>::iterator it2 = temp_blocks.begin(); it2 <= temp_blocks.end(); ++it2)
							{
								if (it2 == temp_blocks.end())
								{
									move_Y = true;
									break;
								}

								if (!(*it2)->HasCollider())
									continue;
								if (it == it2)
									continue;
								if (dynamic_cast<TileEntity*>(*it2)->block_type == TileEntity::SOLID_BLOCK)
								{
									if (CollisionManager::GetInstance()->CheckAABBCollision(*it2, Player::GetInstance()))
									{
										move_Y = false;
										break;
									}
								}
							}

							if (move_X)
								position.x = temp.x;
							if (move_Y)
								position.y = temp.y;

							SetAABB(Vector3((position.x + (maxBoundary.x * 0.5)), (position.y + (maxBoundary.y * 0.5)), (position.z + (maxBoundary.z * 0.5))), Vector3((position.x + (minBoundary.x * 0.5)), (position.y + (minBoundary.y * 0.5)), (position.z + (minBoundary.z * 0.5))));
							break;
						}
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
		if (abs(velocity.x) > 0)
		{
			velocity.x -= (velocity.x * 5) * dt;
			if (velocity.x > -0.01f && velocity.x < 0.01f)
				velocity.x = 0;
			else
				velocity.x = direction.x * Math::Clamp(std::abs(velocity.x), Math::EPSILON, maxvelocity.x);
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

	//this->animation->Render();

   /* this->m_player_equipment[EQUIPMENT_MELEE]->Render();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();*/

	if (!m_moving)
	{
		this->animation->Render();
		//this->animationWalking->Render();
	}
	else if (this->direction.x == 1)
	{
		this->animationWalking->Render();
	}
	else if (this->direction.x == -1)
	{
		this->animationWalkingLeft->Render();
	}
	else if (this->direction.y == 1)
	{
		this->animationClimbing->Render();
	}
	else if (m_jump)
	{
		this->animationJumping->Render();
	}
}


void Player::MoveUp(double dt)
{
    this->direction.y = 1;

	if (m_isClimbing)
	{
		last_direction.y = direction.y;
		this->velocity.y = 1;
		this->accleration.y = 3.8;
		m_movingtimer = m_defmov;
		m_movingtimer = 0.125f;

		this->m_moving = true;
	}
}

void Player::MoveDown(double dt)
{
	this->direction.y = -1;

	std::vector<EntityBase*> temp_blocks;
	EntityManager::GetInstance()->GetAllBlocksWithinTileRadius(tile_ID, temp_blocks, Player::GetInstance()->GetIsFightingBoss());

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
	if (!m_isClimbing && m_isOnFloor)
	{
		last_direction.y = direction.y;
		this->direction.y = 1;
		this->m_jump = true;
		this->m_canDoubleJump = true;
		this->m_isOnFloor = false;
		this->velocity.y = 6;
		this->accleration.y = -9.8f;
	}
	else if (!m_isClimbing && m_canDoubleJump && velocity.y < 0)
	{
		last_direction.y = direction.y;
		this->direction.y = 1;
		this->m_jump = true;
		this->m_canDoubleJump = false;
		this->m_isOnFloor = false;
		this->velocity.y = 6;
		this->accleration.y = -9.8f;
	}
}

void Player::DodgeRoll(double dt)
{
	m_invincible = true;
	this->velocity.x = 20 * this->direction.x;
	this->m_moving = true;
	m_movingtimer = 0.125;
    this->m_invincibletimer = this->m_definvincibletimer;
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

bool Player::IsInteracting()
{
	return m_interacted;
}

void Player::SetInteracting(bool interacting)
{
	m_interacted = interacting;
	m_interacttimer = m_definteracttimer;
}

void Player::TakeDamage(int _dmg)
{
    if (this->m_invincible)
        return;
	this->m_health -= _dmg;
    this->m_invincible = true;
    this->m_invincibletimer = 0.2f;

}

int Player::GetHealth()
{
	return this->m_health;
}

Equipment * Player::GetWeaponInInventory(bool is_primary)
{
	if (is_primary)
		return m_player_equipment[EQUIPMENT_MELEE];

	return m_player_equipment[EQUIPMENT_RANGED];
}

void Player::SetRuneToWeapon(bool is_primary, Runes* rune)
{
	if (is_primary)
	{
		m_player_equipment[EQUIPMENT_MELEE]->addRune(rune);
	}
	else
	{
		m_player_equipment[EQUIPMENT_RANGED]->addRune(rune);
	}
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
	//std::cout << up << std::endl;
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

bool Player::GetIsFightingBoss()
{
	return m_isFightingBoss;
}

void Player::SetIsFightingBoss(bool is_fighting)
{
	if (m_isFightingBoss != is_fighting)
		m_isFightingBoss = is_fighting;
	else
		m_isFightingBoss = false;

	if (m_isFightingBoss)
	{
		last_position = position;
		m_isFightingBoss = is_fighting;
		position.Set(100, 102, 0);
	}
	else
	{
		m_isFightingBoss = false;
		position.Set(last_position.x, last_position.y + 0.01, last_position.z);
		m_isKilledBoss = true;
		FamineBoss* pb = new FamineBoss();
		pb->SetPosition(Vector3(111, 102, 0));
		pb->Init();
		EntityManager::GetInstance()->AddEntity(pb, true);
	}
}

void Player::SetIsKilledBoss(bool killed_boss)
{
	m_isKilledBoss = true;
}

bool Player::GetIsKilledBoss()
{
	return m_isKilledBoss;
}

void Player::StartNextLevel()
{
	++m_iLevel;
	if (m_iLevel < 4)
	{
		EntityManager::GetInstance()->ResetEntityBase();
		MapManager::GetInstance()->GenerateBlocks(Player::GetInstance()->GetCurrentLevel());
		MapManager::GetInstance()->GenerateBossBlocks(Player::GetInstance()->GetCurrentLevel());
		Player::GetInstance()->SetPosition(MapManager::GetInstance()->GetAllPlayerStartingPos()[Player::GetInstance()->GetCurrentLevel()]);

		m_isKilledBoss = false;
		m_isFightingBoss = false;
	}
	else
	{
		//VICTORY SCREEN
		GameStateManager::GetInstance()->setState(GS_LEVELCOMPLETE);
	}
}

void Player::ResetGame()
{
	this->m_health = 100;
}
