#ifndef _PLAGUEBUBBLESTRATEGY_H
#define _PLAGUEBUBBLESTRATEGY_H

#include "PlagueStrategy.h"

#include <vector>

class PoisonGasBubbles;

class PlagueBubbleStrategy : public PlagueStrategy
{
public:
    PlagueBubbleStrategy();
    virtual ~PlagueBubbleStrategy();

    virtual void Init();
    virtual void Update(double dt);
protected:
	std::vector<PoisonGasBubbles*> bubbles;
};

#endif