#include "PoisonGasBubble.h"
#include "MapManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "PlagueBoss.h"
#include "ToxicGas.h"

#include "GraphicsManager.h"
#include "RenderHelper.h"

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
    result->SetTileID(1);
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

void PoisonGasBubbles::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir, PlagueBoss* _parent)
{
	this->position = _pos;
	this->velocity = _vel;
	this->direction = _dir;
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->isStatic = false;
	this->m_bCollider = true;
	this->type = PLAGUE_BUBBLE_OBJ;
	this->m_active = true;
	float sc = Math::RandIntMinMax(2, 5);
	this->m_maxscale.Set(sc, sc, 1);
	this->m_defbubbletimer = Math::RandFloatMinMax(5, 10);
	this->m_bubbletimer = 0;
	this->m_pop = false;
	this->size.Set(0.25, 0.25, 0);
	this->m_bCollider = true;
    this->m_parent = _parent;
	m_dmg = 5;
}

void PoisonGasBubbles::Update(double _dt)
{
	if (!this->m_active)
		return;
	
	if(size.x < m_maxscale.x)
		this->size += Vector3(0.1, 0.1) * _dt;


	this->m_bubbletimer += _dt;
	if (m_bubbletimer > m_defbubbletimer)
	{
		m_pop = true;
	}
    if (m_pop)
        this->Pop();
        
	this->position += velocity * _dt;
	GenerateAABB(this->position);
}

void PoisonGasBubbles::Render()
{
	if (!this->m_active)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z + 0.14);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

bool PoisonGasBubbles::CollisionResponse(GenericEntity * ThatEntity)
{
	if (!this->m_active)
		return false;

	if (ThatEntity->type == PLAYER_OBJ)
	{
		this->Pop();
		Player* ThatEntity1 = dynamic_cast<Player*>(ThatEntity);
		ThatEntity1->TakeDamage(m_dmg);
	}
	return false;
}

void PoisonGasBubbles::Pop()
{
	this->m_active = false;
	this->m_bCollider = false;
	this->isStatic = true;
    GenericEntity* ge = this->m_parent->GetEntity(GenericEntity::PLAGUE_GAS_OBJ);
    if (!ge)
        return;
    ToxicGas* tg = dynamic_cast<ToxicGas*>(ge);
	if (tg)
	{
		std::cout << "POP!" << std::endl;
		tg->Init(this);
	}
}
