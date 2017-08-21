#include "Ranged.h"
#include "../Projectile.h"

Ranged::Ranged(): m_projectilecount(10)
{
	m_projectilelist = new Projectile*[m_projectilecount];
}

Ranged::Ranged(int _projectilecount): m_projectilecount(_projectilecount)
{
	m_projectilelist = new Projectile*[m_projectilecount];
}

Ranged::Ranged(int level, RUNE_TYPE type) : m_projectilecount(10)
{
	m_projectilelist = new Projectile*[m_projectilecount];
	//TODO: Randomise stats based on constructor
}

Ranged::~Ranged()
{
}

Projectile * Ranged::GetProjectile()
{
	if (!m_projectilelist)
		return nullptr;

	Projectile* p = nullptr;

	for (int i = 0; i < m_projectilecount;++i)
	{
		if (!(this->m_projectilelist[i]->GetActive()))
		{
			p = this->m_projectilelist[i];
			break;
		}
	}

	return p;
}


