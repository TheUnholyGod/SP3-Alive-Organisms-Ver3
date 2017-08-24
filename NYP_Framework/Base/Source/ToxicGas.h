#ifndef _TOXICGAS_H
#define _TOXICGAS_H

#include "Hitbox.h"

class ToxicGas : public Hitbox
{
public:
    ToxicGas(Mesh* _modelMesh);
    virtual ~ToxicGas();

    virtual void Init();
    virtual void Update(double _dt);
    virtual void Render();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);
};

namespace Create
{
    ToxicGas* CreatePoisonHitbox(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		bool is_boss_room = true);
};

#endif