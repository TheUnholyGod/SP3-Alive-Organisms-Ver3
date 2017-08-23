#ifndef _RANGED_H_
#define _RANGED_H_
#include "Equipment.h"

class Projectile;

class Ranged:public Equipment
{
public:
	enum DIR
	{	
		DIR_LEFT,
		DIR_RIGHT,
		DIR_UP,
		DIR_DOWN,
	
		NUM_DIR,
	};
	Ranged();
	Ranged(int _projectilecount);
	Ranged(int level, RUNE_TYPE type);
	virtual ~Ranged();

	virtual void Init(Player* _owner = nullptr) = 0;
	virtual void Action(int _actiontype = 0) = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual Projectile* GetProjectile();
	
protected:
	Projectile** m_projectilelist;
	int m_projectilecount;
	int m_curr_dir;
};
#endif