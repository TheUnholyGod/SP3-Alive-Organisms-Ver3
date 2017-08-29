#include "Bow.h"
#include "Projectile.h"
#include "MeshList.h"
#include "PlayerInfo\PlayerInfo.h"

Bow::Bow() : Ranged()
{
}

Bow::~Bow()
{
}

void Bow::Init(Player * _owner)
{
	this->m_owner = _owner;
	for (int i = 0; i < m_projectilecount; ++i)
	{
		this->m_projectilelist[i] = Create::CreateProjectile("arrow_projectile", Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), this);
		this->m_projectilelist[i]->SetSize(Vector3(1, 1, 1));
	}
}

void Bow::Action(int _actiontype)
{
	Projectile* p = this->GetProjectile();
	if (p)
	{
		p->SetActive(true);
		if(_actiontype >= DIR_UP && _actiontype <= DIR_DOWN)
			p->Init(this->m_owner->GetPosition() + (this->m_owner->GetDirection().y * Vector3(0, 0.25)), Vector3(0,10 * m_owner->GetDirection().y), Vector3(0,m_owner->GetDirection().y));
		else
			p->Init(this->m_owner->GetPosition() + (this->m_owner->GetDirection().x * Vector3(0.25)),Vector3(10 * m_owner->GetDirection().x), Vector3(m_owner->GetDirection().x));
	}
}

void Bow::Update(double dt)
{
}

void Bow::Render()
{
}
