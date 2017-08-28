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
}

bool TileEntity::CollisionResponse(GenericEntity* other)
{
	return false;
}

