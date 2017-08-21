#include "Collider.h"

//<Remove>
#include "../GraphicsManager.h"
#include "../MatrixStack.h"
#include "../RenderHelper.h"
#include "../MeshBuilder.h"
//</Remove>

Collision::Collision()
	: maxAABB(Vector3( 0, 0, 0))
	, minAABB(Vector3( 0, 0, 0))
    , m_mesh(MeshList::GetInstance()->GetMesh("q"))
{
}


Collision::~Collision()
{
}

// Set the maxAABB and minAABB
void Collision::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
	if (!m_bAABB)
		m_bAABB = true;
}
// Set the maxAABB
void Collision::SetMaxAABB(Vector3 maxAABB)
{
	this->maxAABB = maxAABB;
}
// Get the maxAABB
Vector3 Collision::GetMaxAABB(void)
{
	return maxAABB;
}
// Set the minAABB
void Collision::SetMinAABB(Vector3 minAABB)
{
	this->minAABB = minAABB;
}
// Get the minAABB
Vector3 Collision::GetMinAABB(void)
{
	return minAABB;
}

void Collision::SetRadius(float _value)
{
	radius = _value;
	if (!m_bSphere)
		m_bSphere = true;
}

float Collision::GetRadius()
{
	return radius;
}

Vector3 Collision::GetSize()
{
    return this->size;
}

void Collision::SetSize(Vector3 _size)
{
    this->size = _size;
}

void Collision::GenerateAABB(Vector3 _pos)
{
    this->maxAABB = _pos + (this->size * 0.5f);
    this->minAABB = _pos - (this->size * 0.5f);
}

bool Collision::HasSphere()
{
	return m_bSphere;
}

bool Collision::HasAABB()
{
	return m_bAABB;
}

void Collision::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate((this->maxAABB.x + this->minAABB.x)/2, (this->maxAABB.y + this->minAABB.y)/2, (this->maxAABB.z + this->minAABB.z)/2);
    modelStack.Scale((this->maxAABB.x - this->minAABB.x), (this->maxAABB.y - this->minAABB.y), (this->maxAABB.z - this->minAABB.z));
    RenderHelper::RenderMesh(m_mesh);
    modelStack.PopMatrix();
}
