#ifndef _PLAGUEPROJECTILESTRATEGY_H
#define _PLAGUEPROJECTILESTRATEGY_H

#include "PlagueStrategy.h"

class PlagueProjectileStrategy : public PlagueStrategy
{
public:
    PlagueProjectileStrategy();
    virtual ~PlagueProjectileStrategy();

    virtual void Init();
    virtual void Update(double dt);
};

#endif