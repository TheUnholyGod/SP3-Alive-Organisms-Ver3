#ifndef _TOXICGAS_H
#define _TOXICGAS_H

#include "Hitbox.h"

class PoisonGasBubbles;

class ToxicGas : public Hitbox
{
public:
    ToxicGas(Mesh* _modelMesh);
    virtual ~ToxicGas();

    virtual void Init(PoisonGasBubbles* _parent);
    virtual void Update(double _dt);
    virtual void Render();
	virtual void Exit();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);

};

namespace Create
{
    ToxicGas* CreateToxicGas(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		bool is_boss_room = true);
};

#endif