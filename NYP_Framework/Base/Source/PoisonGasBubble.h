#ifndef _POISONGASBUBBLES_H
#define _POISONGASBUBBLES_H

#include "Projectile.h"

class PoisonGasBubbles : public Projectile
{
public:
    PoisonGasBubbles();
    PoisonGasBubbles(Mesh* _mesh);
    virtual ~PoisonGasBubbles();

    virtual void Init(Vector3 _pos = Vector3(0, 0, 0), Vector3 _vel = (0, 0, 0), Vector3 _dir = Vector3(0, 0, 0));

    virtual void Update(double _dt);
    virtual void Render();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);
};

namespace Create
{
    PoisonGasBubbles* CreatePoisonGasBubbles(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        Ranged* _parent = nullptr);
};

#endif
