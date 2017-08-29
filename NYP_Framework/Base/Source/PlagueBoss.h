#ifndef _PLAGUEBOSS_H
#define _PLAGUEBOSS_H

#include "Enemy\EnemyBossBase.h"
#include <vector>

class PlagueStrategy;
class SpriteEntity;

class PlagueBoss : public EnemyBossBase
{
private:
    enum PLAGUESTATES
    {
        STATE_IDLE,
        STATE_SUMMON,
        STATE_BUBBLE,
        STATE_CHARGE,
		STATE_DIE,
        NUM_STATES,
    }m_currstate;

    PlagueStrategy** m_strats;

	std::vector<GenericEntity*> m_entitylist;
	bool m_isstatefree[NUM_STATES];

	bool m_freestate;
	double m_defchangestatetimer, m_changestatetimer;
	SpriteEntity* idle;
	SpriteEntity* charge;
	SpriteEntity* action;
	SpriteEntity* die;

public:
    PlagueBoss();
    PlagueBoss(Mesh* _mesh);
    ~PlagueBoss();

    virtual void Init();
    virtual void Update(double _dt);
    virtual void Render();
    virtual bool CollisionResponse(GenericEntity*);

	virtual bool GetNextState();

	virtual GenericEntity* GetEntity(GenericEntity::OBJECT_TYPE _type);
};

#endif