#ifndef _POISONHITBOX_H
#define _POISONHITBOX_H

#include "Hitbox.h"

class PoisonHitbox : public Hitbox
{
public:
    PoisonHitbox(Mesh* _modelMesh);
    virtual ~PoisonHitbox();

    virtual void Init();
    virtual void Update(double _dt);
    virtual void Render();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);
};

namespace Create
{
    PoisonHitbox* CreatePoisonHitbox(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		bool is_boss_room = true);
};

#endif