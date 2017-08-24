#include "PoisonGasBubble.h"
#include "MapManager.h"
#include "PlayerInfo\PlayerInfo.h"

PoisonGasBubbles * Create::CreatePoisonGasBubbles(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent, bool is_boss)
{
	return nullptr;
}

PoisonGasBubbles::PoisonGasBubbles()
{
}

PoisonGasBubbles::PoisonGasBubbles(Mesh * _mesh)
{
}

PoisonGasBubbles::~PoisonGasBubbles()
{
}

void PoisonGasBubbles::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir)
{
	this->position = _pos;
	this->velocity = _vel;
	this->direction = _dir;
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position);
	this->isStatic = false;
	this->m_bCollider = true;
	this->size.Set(0.5f, 0.5f, 1);
	this->type = PROJECTILE_OBJ;
	//this->m_active = true;
	float sc = Math::RandIntMinMax(2, 5);
	this->m_maxscale.Set(sc, sc, 1);
	this->m_defbubbletimer = Math::RandFloatMinMax(3, 10);
	this->m_bubbletimer = 0;
	this->m_pop = false;
}

void PoisonGasBubbles::Update(double _dt)
{
	if (!this->m_active)
		return;
	
	this->m_bubbletimer += _dt;
	if (m_bubbletimer > m_defbubbletimer)
	{
		m_active = false;
		m_pop = true;
	}
	this->position += velocity * _dt;
}

void PoisonGasBubbles::Render()
{
	Collision::Render();
}

bool PoisonGasBubbles::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}
