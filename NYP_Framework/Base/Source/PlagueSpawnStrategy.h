#ifndef _PLAGUESPAWNSTRATEGY_H
#define _PLAGUESPAWNSTRATEGY_H

#include "PlagueStrategy.h"

class PlagueSpawnStrategy : public PlagueStrategy
{
public:
    PlagueSpawnStrategy();
    virtual ~PlagueSpawnStrategy();

    virtual void Init();
    virtual void Update(double dt);
};

#endif