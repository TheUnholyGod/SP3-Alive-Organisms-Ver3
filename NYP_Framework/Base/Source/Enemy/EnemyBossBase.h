#ifndef _ENEMYBOSSBASE_H
#define _ENEMYBOSSBASE_H

#include "../GenericEntity.h"

class EnemyBossBase :public GenericEntity
{
protected:
	bool done_dead;
public:
	enum TYPE
	{
		TYPE_PLAGUE,
		NUM_TYPE
	};
	EnemyBossBase();
	~EnemyBossBase();

    virtual void Init();
    virtual void Update(double _dt);
    virtual void Render();
    virtual bool CollisionResponse(GenericEntity*);
	virtual void OnDead();
};

#endif