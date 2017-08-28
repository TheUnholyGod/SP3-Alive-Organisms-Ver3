#include "Projectile.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Mtx44.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

Projectile::Projectile() : GenericEntity(nullptr)
{
	m_active = false;
}

Projectile::Projectile(Mesh * _mesh) : GenericEntity(_mesh)
{
	m_active = false;
}

Projectile::~Projectile()
{

}

void Projectile::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir)
{
	this->position = _pos;
	this->velocity = _vel;
	this->direction = _dir;
	this->tile_ID = -1;
	this->isStatic = false;
	this->m_bCollider = true;
	this->size.Set(0.5f, 0.5f, 1);
	this->type = PROJECTILE_OBJ;
	this->m_active = true;
}

void Projectile::Update(double _dt)
{
	if (!m_active)
		return;
	this->position += velocity * _dt;
	this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->GenerateAABB(this->position);
}

void Projectile::Render()
{
	if (!m_active)
		return;

	Collision::Render();
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(this->position.x, this->position.y, this->position.z);
	modelStack.Scale(this->scale.x, this->scale.y, this->scale.z);
	RenderHelper::RenderMesh(this->modelMesh);
	modelStack.PopMatrix();
}

void Projectile::Exit()
{
}

bool Projectile::CollisionResponse(GenericEntity * ThatEntity)
{
	if (ThatEntity->type == ENEMY_OBJ)
	{
		ThatEntity->SetIsDone(true);
	}

	this->m_active = false;
	this->m_bCollider = false;
	this->tile_ID = 0;

	return false;
}

bool Projectile::GetActive()
{
	return this->m_active;
}

void Projectile::SetActive(bool _active)
{
	this->m_active = _active;
}

Projectile * Create::CreateProjectile(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, Ranged * _parent,
	bool is_boss_room)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Projectile* result = new Projectile(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetTileID(-1);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}

