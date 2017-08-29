#include "ArchProjectile.h"
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

void Projectile::Init(Vector3 _pos, Vector3 _vel, Vector3 _dir,bool _isplayer)
{
	this->scale.Set(2, 2,0);
	this->position = _pos;
	this->velocity = _vel;
	this->direction = _dir;
	this->tile_ID = -1;
	this->isStatic = false;
	this->m_bCollider = true;
	this->size.Set(0.25f, 0.25f, 0.25f);
	this->type = PROJECTILE_OBJ;
	this->m_active = true;
	this->m_isplayer = _isplayer;
	this->m_dmg = 5;
	this->m_dist.SetZero();
	this->m_range.Set(25,25,0);
}

void Projectile::Update(double _dt)
{
	if (!m_active)
		return;
	this->position += velocity * _dt;
	this->m_dist += velocity * _dt;
	if (this->m_dist.x > this->m_range.x || this->m_dist.y > this->m_range.y)
	{
		this->m_active = false;
		this->m_bCollider = false;
		this->isStatic = true;
		this->tile_ID = 0;
	}
	//this->tile_ID = MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(this->position, Player::GetInstance()->GetIsFightingBoss());
	this->GenerateAABB(this->position);
}

void Projectile::Render()
{
	if (!m_active)
		return;

	//Collision::Render();
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
	if (m_isplayer)
	{
		if (ThatEntity->type == ENEMY_OBJ)
		{
			ThatEntity->ApplyDamage(Player::GetInstance()->GetDamage());
			this->m_active = false;
		}
		if (ThatEntity->type == PLAYER_OBJ)
		{
			return false;
		}
	}
	else
	{
		if (ThatEntity->type == PLAYER_OBJ)
		{
			Player* ThatEntity1 = dynamic_cast<Player*>(ThatEntity);
			ThatEntity1->TakeDamage(m_dmg);
		}
	}


	this->m_active = false;
	this->m_bCollider = false;
	this->isStatic = true;
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

