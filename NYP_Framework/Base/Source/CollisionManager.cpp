#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "MapManager.h"

bool CollisionManager::CheckPointToSphereCollision(Vector3 point, EntityBase * ThatEntity)
{
	if (!ThatEntity->HasCollider()) {
		std::cout << "Entity does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thatSphere = dynamic_cast<GenericEntity*>(ThatEntity);
	
	if (!thatSphere->HasSphere()) {
		std::cout << "Entity does not have Bounding Sphere" << std::endl;
		return false;
	}

	if (DistanceSquaredBetween(point, thatSphere->GetPosition() < thatSphere->GetRadius() * thatSphere->GetRadius()))
		return true;
	return false;
}

bool CollisionManager::CheckSphereCollision(EntityBase * ThisEntity, EntityBase * ThatEntity)
{
	if (!ThisEntity->HasCollider() || !ThatEntity->HasCollider()) {
		std::cout << "1 or more Entities does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thisSphere = dynamic_cast<GenericEntity*>(ThisEntity);
	GenericEntity* thatSphere = dynamic_cast<GenericEntity*>(ThatEntity);
	
	if (!thatSphere->HasSphere() || !thisSphere->HasSphere()) {
		std::cout << "1 or more Entities does not have Bounding Sphere" << std::endl;
		return false;
	}

	float totalRadius = thatSphere->GetRadius() + thisSphere->GetRadius();
	if (DistanceSquaredBetween(thatSphere->GetPosition(), thisSphere->GetPosition()) < totalRadius * totalRadius) {
		return true;
	}
	return false;
}

bool CollisionManager::CheckAABBCollision(EntityBase * ThisEntity, EntityBase * ThatEntity)
{
	if (!ThatEntity->HasCollider() || !ThisEntity->HasCollider()) {
	//	std::cout << "1 or more Entities does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thisHitbox = dynamic_cast<GenericEntity*>(ThisEntity);
	GenericEntity* thatHitbox = dynamic_cast<GenericEntity*>(ThatEntity);

	if (!thisHitbox->HasAABB() || !thatHitbox->HasAABB()){
	//	std::cout << "1 or more Entities does not have AABB" << std::endl;
		return false;
	}

	return (thisHitbox->GetMinAABB().x <= thatHitbox->GetMaxAABB().x && thisHitbox->GetMaxAABB().x >= thatHitbox->GetMinAABB().x) &&
		(thisHitbox->GetMinAABB().y <= thatHitbox->GetMaxAABB().y && thisHitbox->GetMaxAABB().y >= thatHitbox->GetMinAABB().y) &&
		(thisHitbox->GetMinAABB().z <= thatHitbox->GetMaxAABB().z && thisHitbox->GetMaxAABB().z >= thatHitbox->GetMinAABB().z);
}

bool CollisionManager::CheckPointToAABBCollision(Vector3 point, EntityBase * ThatEntity, bool is2D)
{
	if (!ThatEntity->HasCollider()) {
		//std::cout << "Entity does not have Collider" << std::endl;
		return false;
	}

	GenericEntity* thatHitbox = dynamic_cast<GenericEntity*>(ThatEntity);

	if (!thatHitbox->HasAABB()) {
		//std::cout << "Entity does not have AABB" << std::endl;
		return false;
	}

	if (!is2D)
		return (point.x <= thatHitbox->GetMaxAABB().x && point.x >= thatHitbox->GetMinAABB().x) &&
		(point.y <= thatHitbox->GetMaxAABB().y && point.y >= thatHitbox->GetMinAABB().y) &&
		(point.z <= thatHitbox->GetMaxAABB().z && point.z >= thatHitbox->GetMinAABB().z);
	else
		return (point.x <= thatHitbox->GetMaxAABB().x && point.x >= thatHitbox->GetMinAABB().x) &&
		(point.y <= thatHitbox->GetMaxAABB().y && point.y >= thatHitbox->GetMinAABB().y);
}

bool CollisionManager::CheckPlayerDirCollision(EntityBase * tile_entity)
{
	//Vector3 w0 = tile_entity->GetPosition();
	//Vector3 b1 = Player::GetInstance()->GetPosition();
	//float r = Player::GetInstance()->GetScale().y;
	//float h = tile_entity->GetScale().y;
	//float l = tile_entity->GetScale().x;
	//Vector3 N = dynamic_cast<TileEntity*>(tile_entity)->GetDir();
	//Vector3 NP = dynamic_cast<TileEntity*>(tile_entity)->GetDir().Cross(Vector3(0, 0, 1));
	//Vector3 relative_position = Player::GetInstance()->GetPosition() - tile_entity->GetPosition();

	///*if (relative_position.Dot(N) < 0)
	//{
	//	N = -N;
	//}*/

	//return Player::GetInstance()->GetPlayerVelocity().Dot(N) > 0 && (abs((w0 - b1).Dot(N)) < r + h * 0.5f) && (abs((w0 - b1).Dot(NP)) < r + l * 0.5f);
	if (Player::GetInstance()->GetDirection().y > 0)
	{
		return false;
	}

	return true;
}

void CollisionManager::Update(std::list<EntityBase*> collisionList)
{
	int i = 0;
	std::list<EntityBase*>::iterator it, it2;

	for (it = collisionList.begin(); it != collisionList.end(); ++it)
	{

		if ((*it)->GetIsStatic())
			break;

		for (it2 = std::next(it, 1); it2 != collisionList.end(); ++it2)
		{
			++i;
			if (!(*it)->HasCollider() || !(*it2)->HasCollider())
			{
				continue;
			}

			if (CheckAABBCollision(*it, *it2))
			{
				GenericEntity* thisEntity = dynamic_cast<GenericEntity*>(*it);
				GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(*it2);
				//create collison response code to settle what to do
				thisEntity->CollisionResponse(thatEntity);
			}
		}
	}

	for (it = collisionList.begin(); it != collisionList.end(); ++it)
	{
		if (!(*it)->HasCollider())
			continue;

		if (CheckAABBCollision(*it, Player::GetInstance()))
		{
			GenericEntity* thatEntity = dynamic_cast<GenericEntity*>(Player::GetInstance());
			GenericEntity* thisEntity = dynamic_cast<GenericEntity*>(*it);
			//create collison response code to settle what to do
			thisEntity->CollisionResponse(thatEntity);
		}
	}
}

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}
