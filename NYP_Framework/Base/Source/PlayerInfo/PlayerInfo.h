#pragma once

#include <vector>

#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "SingletonTemplate.h"
#include "../GenericEntity.h"
#include "../Items/Equipment.h"

class SpriteEntity;

class Player : public Singleton<Player> , public GenericEntity
{
	friend Singleton<Player>;
    enum EQUIPMENT
    {
        EQUIPMENT_MELEE,
        EQUIPMENT_RANGED,
        EQUIPMENT_FLASK,
        NUM_EQUIPMENT,
    };
public:
	Player();
	~Player();

	// Initialise this class instance
	void Init(void);
	// Update
	void Update(double dt = 0.0333f);
	void UpdateMovement(double dt);
	void UpdateJump(double dt);
	//Render 
	void Render();

	//Movement
	void MoveUp(double dt);
	void MoveDown(double dt);
	void MoveRight(double dt);
	void MoveLeft(double dt);
	void Jump(double dt);
	void DodgeRoll(double dt);
	void PrimaryAttack(double dt, int _combo);
	void SecondaryAttack(double dt, int _actiontype = 0);
	bool IsInteracting();
	void SetInteracting(bool);

	void TakeDamage(int _dmg);
	int GetHealth();
	Equipment*  GetWeaponInInventory(bool);
	void SetRuneToWeapon(bool, Runes*);

	// Handling Camera
	FPSCamera* getCamera();
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

	int GetCurrentLevel();

	Vector3 GetDirection();
	Vector3 GetPlayerVelocity();
	bool GetIsFightingBoss();
	void SetIsFightingBoss(bool);
	void SetIsKilledBoss(bool);
	bool GetIsKilledBoss();

	void StartNextLevel();
	void ResetGame();
private:
	Vector3 defaultPosition;
	Vector3 direction, velocity, accleration, maxvelocity, maxofthemaxvelocity;
	Vector3 last_direction, last_position;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	int m_iLevel;
	double m_combotimer;
	double m_regenTimer;
	bool m_attacking;
	int m_combo;
	double m_dSpeed;
	double m_dAcceleration;

	int m_health, m_maxHealth;

	bool m_jump;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dElapsedTime;
	double m_movingtimer,m_defmov;
	bool m_bFallDownwards;
	bool m_moving;
	bool m_invincible;
    double m_invincibletimer, m_definvincibletimer;

	bool m_isOnFloor;
	bool m_isClimbing;
    bool m_interacted;
    double m_interacttimer, m_definteracttimer;
	bool m_isFightingBoss, m_isKilledBoss;
	bool m_canDoubleJump;

	Equipment* m_player_equipment[NUM_EQUIPMENT];

	FPSCamera* attachedCamera;

	SpriteEntity* animation;
	SpriteEntity* animationWalking;
	SpriteEntity* animationWalkingLeft;
	SpriteEntity* animationClimbing;
	SpriteEntity* animationJumping;
};
