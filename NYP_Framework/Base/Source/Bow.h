#ifndef _BOW_H
#define _BOW_H

#include "Items\Ranged.h"

class Bow : public Ranged
{
public:
	Bow();
	virtual ~Bow();

	virtual void Init(Player* _owner = nullptr);
	virtual void Action(int _actiontype = 0);
	virtual void Update(double dt);
	virtual void Render();
};

#endif