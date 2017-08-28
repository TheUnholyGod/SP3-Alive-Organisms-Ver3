#ifndef _FAMINE_MELEE_STRATEGY_H_
#define _FAMINE_MELEE_STRATEGY_H_

#include "FamineStrategy.h"
class Hitbox;

class FamineMeleeStrategy : public FamineStrategy
{
public:
	FamineMeleeStrategy();
	virtual ~FamineMeleeStrategy();

	virtual void Init();
	virtual void Update(double dt);
protected:
	Hitbox* m_meleeatt;
};

#endif