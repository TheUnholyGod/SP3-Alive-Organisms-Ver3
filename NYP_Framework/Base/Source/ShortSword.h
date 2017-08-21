#ifndef _SHORTSWORD_H
#define _SHORTSWORD_H

#include "Items\Melee.h"
class Player;

class ShortSword :public Melee
{
public:
    ShortSword();
    ShortSword(int level, RUNE_TYPE type);
    virtual ~ShortSword();

    virtual void Init(Player* _owner = nullptr);
    virtual void Action(int _combo);
    virtual void Update(double dt);
    virtual void Render();

};

#endif