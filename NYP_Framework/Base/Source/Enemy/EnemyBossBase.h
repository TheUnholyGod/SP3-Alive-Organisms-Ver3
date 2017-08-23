#ifndef _ENEMYBOSSBASE_H
#define _ENEMYBOSSBASE_H

#include "../GenericEntity.h"

class EnemyBossBase :public GenericEntity
{
private:

public:
	enum TYPE
	{
		TYPE_PLAGUE,
		NUM_TYPE
	};
	EnemyBossBase();
	~EnemyBossBase();
};

#endif