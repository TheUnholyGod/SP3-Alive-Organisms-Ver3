#ifndef _PLAGUECHARGESTRATEGY_H
#define _PLAGUECHARGESTRATEGY_H

#include "PlagueStrategy.h"
#include "Vector3.h"

#include <vector>

class PoisonHitbox;

class PlagueChargeStrategy : public PlagueStrategy
{
public:
    PlagueChargeStrategy();
    virtual ~PlagueChargeStrategy();

    virtual void Init();
    virtual void Update(double dt);
protected:
	std::vector<PoisonHitbox*> m_poison;
	int m_charges;
	Vector3 m_velocity, m_direction;
};

#endif