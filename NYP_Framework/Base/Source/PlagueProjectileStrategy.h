#ifndef _PLAGUEPROJECTILESTRATEGY_H
#define _PLAGUEPROJECTILESTRATEGY_H

#include "PlagueStrategy.h"

#include <vector>

class PoisonProjectile;

class PlagueProjectileStrategy : public PlagueStrategy
{
public:
    PlagueProjectileStrategy();
    virtual ~PlagueProjectileStrategy();

    virtual void Init();
    virtual void Update(double dt);
protected:
	std::vector<PoisonProjectile*> m_poisonproj;
};

#endif