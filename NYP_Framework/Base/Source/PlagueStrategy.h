#ifndef _PLAGUESTRATEGY_H
#define _PLAGUESTRATEGY_H

#include "Enemy\StrategyBase.h"

class PlagueBoss;

class PlagueStrategy : public StrategyBase
{
public:
    PlagueStrategy();
    virtual ~PlagueStrategy();

    virtual void Init() = 0;
    virtual void Update(double dt) = 0;
protected:
    PlagueBoss* m_parent;
};

#endif