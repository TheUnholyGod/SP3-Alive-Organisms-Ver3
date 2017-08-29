///////////////////////////////////////////////////////////////////////////////
/*!
\file   ParticleEffect.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for ParticleEffect class.
*/
///////////////////////////////////////////////////////////////////////////////
#include "ParticleEffect.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "../MapManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"

ParticleEffect::ParticleEffect(Vector3 pos, Vector3 vel, EFFECT_TYPE type, double effect_life_span, double particle_life_span) //: GenericEntity(MeshList::GetInstance()->GetMesh("quad"))
{
	this->m_emitter = new Emitter(type, pos, vel, effect_life_span, particle_life_span);
	position = pos;
	tile_ID = -1;
}

void ParticleEffect::Update(double _dt)
{
	if (this->getEmitter()->getIsDone())
		this->SetIsDone(true);

	this->m_emitter->update(_dt);
	/*std::cout << "TILEID PARTILE: " << tile_ID << std::endl;
	std::cout << "TILEID PLAYER: " << Player::GetInstance()->GetTileID() << std::endl;*/

}

void ParticleEffect::Render()
{
	this->m_emitter->render();
}

void ParticleEffect::setMesh(Mesh * meshName)
{
	this->m_modelMesh = meshName;
}

Mesh * ParticleEffect::getMesh()
{
	return this->m_modelMesh;
}

Emitter * ParticleEffect::getEmitter()
{
	return this->m_emitter;
}

ParticleEffect * Create::Particle(
	const std::string& _meshName,
	const Vector3& pos,
	const Vector3& vel,
	const EFFECT_TYPE& type,
	const double& effect_life_span,
	const double& particle_life_span,
	bool is_boss_room)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ParticleEffect* result = new ParticleEffect(pos, vel, type, effect_life_span, particle_life_span);
	result->getEmitter()->setMesh(modelMesh);
	result->SetCollider(false); //Disable collision for this
	result->SetPhysic(false);
	result->SetStatic(false);
	result->SetIsParticleEmitter(true);

	//Adds it to entityManager
	EntityManager::GetInstance()->AddEntity(result, is_boss_room);
	return result;
}
