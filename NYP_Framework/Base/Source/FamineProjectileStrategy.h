#ifndef _FAMINE_PROJECTILE_STRATEGY_H_
#define _FAMINE_PROJECTILE_STRATEGY_H_

#include "FamineStrategy.h"

class FamineProjectileStrategy : public FamineStrategy
{
public:
	FamineProjectileStrategy();
	virtual ~FamineProjectileStrategy();

	virtual void Init();
	virtual void Update(double dt);
};

#endif