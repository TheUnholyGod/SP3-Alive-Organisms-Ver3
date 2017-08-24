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

	virtual bool GetIsDone();
	virtual void SetParent(PlagueBoss* _parent);
protected:
    PlagueBoss* m_parent;
	double m_deftotaltimer, m_totaltimer;
	double m_defactiontimer, m_actiontimer;
	bool action;
	bool m_isdone;

};

#endif