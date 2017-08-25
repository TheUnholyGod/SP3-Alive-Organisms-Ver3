#include "PoisonGasBubble.h"
#include "MapManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MeshList.h"
#include "EntityManager.h"

PoisonGasBubbles * Create::CreatePoisonGasBubbles(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent, bool is_boss)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	PoisonGasBubbles* result = new PoisonGasBubbles(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result, is_boss);
	return result;
}

PoisonGasBubbles::PoisonGasBubbles()
{
}

PoisonGasBubbles::PoisonGasBubbles(Mesh * _mesh) :Projectile(_mesh)
{
	this->type = GenericEntity::PLAGUE_BUBBLE_OBJ;
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
	this->type = PLAGUE_BUBBLE_OBJ;
	//this->m_active = true;
	float sc = Math::RandIntMinMax(2, 5);
	this->m_maxscale.Set(sc, sc, 1);
	this->m_defbubbletimer = Math::RandFloatMinMax(15, 30);
	this->m_bubbletimer = 0;
	this->m_pop = false;
	this->size.Set(0.25, 0.25, 0);
	this->isStatic = false;
	this->m_bCollider = true;
	this->tile_ID = 0;

}

void PoisonGasBubbles::Update(double _dt)
{
	if (!this->m_active)
		return;
	
	this->size += Vector3(0.5, 0.5) * _dt;


	this->m_bubbletimer += _dt;
	if (m_bubbletimer > m_defbubbletimer)
	{
		m_active = false;
		m_pop = true;
	}
	this->position += velocity * _dt;
	GenerateAABB(this->position);
}

void PoisonGasBubbles::Render()
{
	Collision::Render();
}

bool PoisonGasBubbles::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}
