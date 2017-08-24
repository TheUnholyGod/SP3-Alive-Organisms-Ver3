#ifndef _POISONPROJECTILE_H
#define _POISONPROJECTILE_H

#include "Projectile.h"

class PoisonProjectile : public Projectile
{
public:
    PoisonProjectile();
    PoisonProjectile(Mesh* _mesh);
    virtual ~PoisonProjectile();

    virtual void Init(Vector3 _pos = Vector3(0, 0, 0), Vector3 _vel = (0, 0, 0), Vector3 _dir = Vector3(0, 0, 0));

    virtual void Update(double _dt);
    virtual void Render();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);
};

namespace Create
{
    PoisonProjectile* CreatePoisonProjectile(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        Ranged* _parent = nullptr,
		bool is_boss_room = true);
};

#endif