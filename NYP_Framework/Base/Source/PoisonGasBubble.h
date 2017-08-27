#ifndef _POISONGASBUBBLES_H
#define _POISONGASBUBBLES_H

#include "Projectile.h"

class PlagueBoss;

class PoisonGasBubbles : public Projectile
{
public:
    PoisonGasBubbles();
    PoisonGasBubbles(Mesh* _mesh);
    virtual ~PoisonGasBubbles();

    virtual void Init(Vector3 _pos = Vector3(), Vector3 _vel = Vector3(), Vector3 _dir = Vector3(1), PlagueBoss* _parent = nullptr);

    virtual void Update(double _dt);
    virtual void Render();

    virtual bool CollisionResponse(GenericEntity* ThatEntity);

    void Pop();
protected:
	Vector3 m_maxscale;
	double m_bubbletimer,m_defbubbletimer;
	bool m_pop;
    PlagueBoss* m_parent;
};

namespace Create
{
    PoisonGasBubbles* CreatePoisonGasBubbles(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        Ranged* _parent = nullptr,
		bool is_boss_room = true);
};

#endif
