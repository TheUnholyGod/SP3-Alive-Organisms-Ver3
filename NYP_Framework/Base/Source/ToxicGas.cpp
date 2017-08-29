#include "ToxicGas.h"
#include "PoisonGasBubble.h"
#include "MeshList.h"
#include "EntityManager.h"

#include "GraphicsManager.h"
#include "RenderHelper.h"

ToxicGas::ToxicGas(Mesh * _modelMesh) : Hitbox(m_mesh)
{
	this->type = PLAGUE_GAS_OBJ;
	this->m_active = false;
	modelMesh = _modelMesh;
}

ToxicGas::~ToxicGas()
{
}

void ToxicGas::Init(PoisonGasBubbles * _parent)
{
    this->m_active = true;
    this->position = _parent->GetPosition();
    this->m_bCollider = true;
    this->isStatic = false;
	this->size = _parent->GetScale();
	this->GenerateAABB(this->position);
	m_hbtimer = 0;
	m_hbdeftimer = Math::RandFloatMinMax(2, 5);
}

void ToxicGas::Update(double _dt)
{
	this->m_hbtimer += _dt;
	if (this->m_hbtimer > this->m_hbdeftimer)
		this->Exit();
}

void ToxicGas::Render()
{
	if (!m_active)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z + 0.14);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void ToxicGas::Exit()
{
	std::cout << "UNPOP" << std::endl;
	this->m_active = false;
	this->isStatic = true;
	this->m_bCollider = false;
}

bool ToxicGas::CollisionResponse(GenericEntity * ThatEntity)
{
	return false;
}

ToxicGas * Create::CreateToxicGas(const std::string & _meshName, const Vector3 & _position, const Vector3 & _scale, bool is_boss)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ToxicGas* result = new ToxicGas(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->SetTileID(-1);

	EntityManager::GetInstance()->AddEntity(result, is_boss);
	return result;
}
