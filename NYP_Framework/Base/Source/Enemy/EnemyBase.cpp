///////////////////////////////////////////////////////////////////////////////
/*!
\file   EnemyBase.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for EnemyBase.
*/
///////////////////////////////////////////////////////////////////////////////
#include "EnemyBase.h"
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../MapManager.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "RenderHelper.h"

#include "EnemyMelee.h"

void EnemyBase::Update(double _dt)
{	
}

void EnemyBase::Render()
{
}

bool EnemyBase::collisionResponse(GenericEntity *)
{
	return false;
}

EnemyBase * Create::Enemy(const EnemyBase::ENEMY_TYPE enemy_type, 
	const Vector3 & _position, 
	const Vector3 & _scale, 
	bool have_collider, 
	bool have_physic, 
	bool is_static)
{
	switch (enemy_type)
	{
	case EnemyBase::ENEMY_TYPE::E_MELEE:
	{
		Mesh* modelMesh = MeshList::GetInstance()->GetMesh("quad");
		if (modelMesh == nullptr)
			return nullptr;

		EnemyMelee* result = new EnemyMelee(modelMesh, enemy_type, _position, _scale);
		EntityManager::GetInstance()->AddEntity(result);
		return result;
	}
	default:
		break;
	}
}
