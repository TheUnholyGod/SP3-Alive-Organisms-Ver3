#ifndef _STATUSEFFECT_H
#define _STATUSEFFECT_H

class GenericEntity;

class StatusEffect
{
public:
	enum STATUS
	{
		NUM_STATUS,
	}m_type;

	StatusEffect();
	~StatusEffect();

	void Update(double dt);

	void AddStack();
	void ApplyDamage();
protected:
	int m_dps;
	double m_timer,m_totaltimer;
	int m_stack;
	double m_onestack_timer;
	GenericEntity* m_victim;
};

#endif