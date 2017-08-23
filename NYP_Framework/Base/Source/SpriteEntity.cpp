#include "SpriteEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "SpriteAnimation.h"
#include "PlayerInfo\PlayerInfo.h"

SpriteEntity::SpriteEntity(Mesh* _modelMesh) :
	modelMesh(_modelMesh),
	position(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	mode(MODE_2D)
{
    this->isStatic = false;
    this->tile_ID = 0;
    this->position = Player::GetInstance()->GetPosition();
    std::cout << position << std::endl;
    mode = MODE_3D;
}

SpriteEntity::~SpriteEntity()
{
}

void SpriteEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D

  /*  this->position = Player::GetInstance()->GetPosition();
    this->position.x += 1;*/
    SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(this->modelMesh);
    if (sa)
    {
        sa->Update(_dt);
        sa->m_anim->animationActive = true;
        std::cout << sa->m_currentFrame << std::endl;
    }
}

void SpriteEntity::Render()
{
    SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(this->modelMesh);

	if (mode == MODE_2D)
		return;

    if (sa)
    {
        MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
        modelStack.PushMatrix();
        modelStack.Translate(position.x, position.y, position.z);
        modelStack.Scale(scale.x, scale.y, scale.z);
        RenderHelper::RenderSpriteAnnimation(sa);
        modelStack.PopMatrix();
    }
}

void SpriteEntity::RenderUI()
{

    SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(this->modelMesh);

	if (mode == MODE_3D)
		return;

    if (sa)
    {
        MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
        modelStack.PushMatrix();
        modelStack.Translate(position.x, position.y, position.z);
        modelStack.Scale(scale.x, scale.y, scale.z);
        RenderHelper::RenderMesh(sa);
        modelStack.PopMatrix();
    }
}

SpriteEntity* Create::Sprite2DObject(const std::string& _meshName, const Vector3& _position, const Vector3& _scale)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	SpriteEntity* result = new SpriteEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetTextRenderMode(SpriteEntity::MODE_2D);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}

SpriteEntity* Create::Sprite3DObject(const std::string& _meshName, const Vector3& _position, const Vector3& _scale)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	SpriteEntity* result = new SpriteEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetTextRenderMode(SpriteEntity::MODE_3D);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}