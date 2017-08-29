#ifndef _FAMINEBOSS_H
#define _FAMINEBOSS_H

#include "Enemy\EnemyBossBase.h"
#include "SpriteEntity.h"
#include <vector>

class ArchProjectile;

class FamineStrategy;

class FamineBoss : public EnemyBossBase
{
private:
	enum FAMINESTATES
	{
		STATE_IDLE,
		STATE_ATTACK,
		STATE_PROJECTILE,
		NUM_STATES,
	}m_currstate;

	FamineStrategy** m_strats;

	std::vector<GenericEntity*> m_entitylist;
	bool m_isstatefree[NUM_STATES];

	bool m_freestate;	
	double m_defchangestatetimer, m_changestatetimer;

	SpriteEntity* idle;
	SpriteEntity* throwing;
	SpriteEntity* melee;

public:
	FamineBoss();
	~FamineBoss();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual bool CollisionResponse(GenericEntity*);

	virtual bool GetNextState();

	virtual GenericEntity* GetEntity(GenericEntity::OBJECT_TYPE _type);
};

#endif