#ifndef _PLAGUESPAWNSTRATEGY_H
#define _PLAGUESPAWNSTRATEGY_H

#include "PlagueStrategy.h"

#include <vector>

class EnemyMaggot;

class PlagueSpawnStrategy : public PlagueStrategy
{
public:
    PlagueSpawnStrategy();
    virtual ~PlagueSpawnStrategy();

    virtual void Init();
    virtual void Update(double dt);
protected:
	std::vector<EnemyMaggot*> m_maggots;
};

#endif