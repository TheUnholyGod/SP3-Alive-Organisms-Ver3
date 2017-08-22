#pragma once

#include <vector>


#include "Vector3.h"
#include "../FPSCamera.h"
#include "../GroundEntity.h"
#include "SingletonTemplate.h"
#include "../GenericEntity.h"

class Equipment;

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
	void UpdateMovment(double dt);
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
	void SecondaryAttack(double dt);

	// Handling Camera
	FPSCamera* getCamera();
	void AttachCamera(FPSCamera* _cameraPtr);
	void DetachCamera();

	int GetCurrentLevel();

	Vector3 GetDirection();
	Vector3 GetPlayerVelocity();

private:
	Vector3 defaultPosition;
	Vector3 direction,velocity,accleration;
	Vector3 last_direction;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	int m_iLevel;
	double m_combotimer;
	bool m_attacking;
	int m_combo;
	double m_dSpeed;
	double m_dAcceleration;

	bool m_jump;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dElapsedTime;
	double m_movingtimer,m_defmov;
	bool m_bFallDownwards;
	bool m_moving;
	bool m_invincible;

	bool m_isOnFloor;
	bool m_isClimbing;

	FPSCamera* attachedCamera;

	Equipment* m_player_equipment[NUM_EQUIPMENT];
};
