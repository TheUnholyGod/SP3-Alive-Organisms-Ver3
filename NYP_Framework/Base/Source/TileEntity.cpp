#include "TileEntity.h"
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

TileEntity::~TileEntity()
{
}

Vector3 TileEntity::GetDir()
{
	return dir;
}

void TileEntity::Update(double dt)
{
}

void TileEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

bool TileEntity::CollisionResponse(GenericEntity* other)
{
	return false;
}

