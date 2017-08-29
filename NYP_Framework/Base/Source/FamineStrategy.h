#ifndef _FAMINE_STRATEGY_H_
#define _FAMINE_STRATEGY_H_

#include "Enemy\StrategyBase.h"

#include "FamineBoss.h"

class FamineStrategy : public StrategyBase
{
public:
	FamineStrategy();
	virtual ~FamineStrategy();

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;

	virtual bool GetIsDone();
	virtual void SetParent(FamineBoss* _parent);
protected:
	FamineBoss* m_parent;
	double m_deftotaltimer, m_totaltimer;
	double m_defactiontimer, m_actiontimer;
	bool action;
	bool m_isdone;

};

#endif