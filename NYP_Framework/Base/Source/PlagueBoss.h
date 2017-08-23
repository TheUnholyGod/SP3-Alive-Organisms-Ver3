#ifndef _PLAGUEBOSS_H
#define _PLAGUEBOSS_H

#include "Enemy\EnemyBossBase.h"

class PlagueBoss : public EnemyBossBase
{
private:
    enum PLAGUESTATES
    {
        STATE_IDLE,
        STATE_SUMMON,
        STATE_BUBBLE,
        STATE_PROJECTILE,
        STATES_CHARGE,
        NUM_STATES,
    }m_currstate;
public:
    PlagueBoss();
    ~PlagueBoss();

    virtual void Init();
    virtual void Update(double _dt);
    virtual void Render();
    virtual bool CollisionResponse(GenericEntity*);
};

#endif