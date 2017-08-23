#ifndef _MELEE_H_
#define _MELEE_H_

#include "Equipment.h"

class Hitbox;
class Player;

class Melee :public Equipment
{
public:
	enum COMBO
	{
		COMBO_BASIC1,
		COMBO_BASIC2,
		COMBO_BASIC3,

		COMBO_FRONT1,
		COMBO_FRONT2,
		COMBO_FRONT3,

		COMBO_BACK1,
		COMBO_BACK2,
		COMBO_BACK3,

		COMBO_UP1,
		COMBO_UP2,
		COMBO_UP3,

		COMBO_DOWN1,
		COMBO_DOWN2,
		COMBO_DOWN3,

		NUM_COMBO,
	};


	Melee();
	Melee(int level, RUNE_TYPE type);
	virtual ~Melee();

    virtual void Init(Player* _owner = nullptr) = 0;
    virtual void Action(int _combo) = 0;
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;

	virtual Hitbox* GetCurrentHitbox();

protected:
	int m_curr_combo;
	Hitbox** m_hitbox;
};
#endif