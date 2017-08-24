#ifndef _PLAGUECHARGESTRATEGY_H
#define _PLAGUECHARGESTRATEGY_H

#include "PlagueStrategy.h"

class PlagueChargeStrategy : public PlagueStrategy
{
public:
    PlagueChargeStrategy();
    virtual ~PlagueChargeStrategy();

    virtual void Init();
    virtual void Update(double dt);
};

#endif