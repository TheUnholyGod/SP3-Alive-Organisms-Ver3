#include "EnemyMelee.h"

#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"


EnemyMelee::EnemyMelee(Mesh * mesh, 
	EnemyBase::ENEMY_TYPE enemy_type, 
	Vector3 _position, 
	Vector3 _scale, 
	bool have_collider, 
	bool have_physic, 
	bool is_static) : EnemyBase(mesh)
{
	this->SetPosition(_position);
	this->SetScale(_scale);
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position));
	this->SetCollider(have_collider);
	this->SetPhysic(have_physic);
	this->SetStatic(false);

	m_timeSinceLastUpdate = 0;
	m_pathing.readMap(MapManager::GetInstance()->getMapArray());
}

EnemyMelee::~EnemyMelee()
{
}

void EnemyMelee::Update(double _dt)
{
	Vector3 playerpos = Player::GetInstance()->GetPosition();
	/*Update, pathfind, set goals etc*/
	if (position.y < playerpos.y)
		position += Vector3(0, 0.5, 0) * _dt;
	else if(position.y > playerpos.y)
		position += Vector3(0, -0.5, 0) * _dt;
	if (position.x < playerpos.x)
		position += Vector3(0.5, 0, 0) * _dt;
	else if (position.x > playerpos.x)
		position += Vector3(-0.5, 0, 0) * _dt;
	
	m_timeSinceLastUpdate += _dt;

	if (m_timeSinceLastUpdate > 1)
	{
		std::future<std::vector<Coord2D>> result = std::async(std::launch::async, &PathFinder::findPath, 
			&m_pathing, 
			Coord2D((int)position.x, (int)position.y), 
			Coord2D((int)Player::GetInstance()->GetPosition().x, (int)Player::GetInstance()->GetPosition().y));
		m_timeSinceLastUpdate = 0;

		if (result.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			result.get();
	}
	this->SetTileID(MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->ReturnTileViaPos(position));
}

void EnemyMelee::Render()
{
	/*Sprite animation?*/
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

bool EnemyMelee::collisionResponse(GenericEntity *)
{
	/*Collision response for this enemy*/
	return false;
}
