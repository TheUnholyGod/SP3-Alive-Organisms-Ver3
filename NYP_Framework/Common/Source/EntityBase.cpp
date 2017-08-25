#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, isDone(false)
	, m_bCollider(false)
	, IsParticleEmitter(false)
	, tile_ID(-1)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::Update(double _dt)
{
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

bool EntityBase::IsDone()
{
	return isDone;
}

void EntityBase::SetIsDone(bool _value)
{
	isDone = _value;
}

// Check if this entity has a collider class parent
bool EntityBase::HasCollider(void) const
{
	return m_bCollider;
}

int EntityBase::GetTileID()
{
	return tile_ID;

}

bool EntityBase::GetIsStatic()
{
	return isStatic;
}

bool EntityBase::GetIsParticleEmitter()
{
	return this->IsParticleEmitter;
}

// Set the flag to indicate if this entity has a collider class parent
void EntityBase::SetCollider(const bool _value)
{
	m_bCollider = _value;
}

bool EntityBase::HasPhysic(void) const
{
	return m_bPhysic;
}

void EntityBase::SetPhysic(const bool _value)
{
	m_bPhysic = _value;
}

void EntityBase::SetStatic(const bool _value)
{
	this->isStatic = _value;
}

void EntityBase::SetIsParticleEmitter(const bool _value)
{
	this->IsParticleEmitter = _value;
}

void EntityBase::SetTileID(const int _value)
{
	this->tile_ID = _value;
}
