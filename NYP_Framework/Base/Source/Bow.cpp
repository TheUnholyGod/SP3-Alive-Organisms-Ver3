#include "Bow.h"
#include "Projectile.h"
#include "MeshList.h"

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
		this->m_projectilelist[i] = Create::CreateProjectile("sphere", Vector3(0, 0, 0), Vector3(1, 1, 1), this);
		this->m_projectilelist[i]->SetSize(Vector3(1, 1, 1));
	}
}

void Bow::Action(int _actiontype)
{
	Projectile* p = this->GetProjectile();
	if (p)
	{
		p->SetActive(true);
		p->Init();
	}
}

void Bow::Update(double dt)
{
}

void Bow::Render()
{
}
