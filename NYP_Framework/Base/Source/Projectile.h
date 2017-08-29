#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "GenericEntity.h"

class Ranged;

class Projectile : public GenericEntity
{
public: 
	Projectile();
	Projectile(Mesh* _mesh);
	virtual ~Projectile();

	virtual void Init(Vector3 _pos = Vector3(0, 0, 0), Vector3 _vel = (0, 0, 0), Vector3 _dir = Vector3(0, 0, 0), bool _isplayer = true);

	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();


	virtual bool CollisionResponse(GenericEntity* ThatEntity);
	virtual bool GetActive();
	virtual void SetActive(bool _active);
protected:
	bool m_isplayer;
	Vector3 m_range, m_dist;
	Vector3 velocity;
	Vector3 direction;
};

namespace Create
{
	Projectile* CreateProjectile(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		Ranged* _parent = nullptr,
		bool is_boss_room = false);
};

#endif