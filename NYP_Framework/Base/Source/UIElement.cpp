///////////////////////////////////////////////////////////////////////////////
/*!
\file   UIElement.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for UIElement.
*/
///////////////////////////////////////////////////////////////////////////////

#include "UIElement.h"
#include "../Source/Manager/UIManager.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

void UIElement::Init()
{
}

void UIElement::Update(double _dt)
{
}

void UIElement::Render()
{
}

void UIElement::Response()
{
}

GAMESTATE UIElement::getState()
{
	return this->m_parent_state;
}

UIElement * Create::UI(const std::string & _meshName, 
	const UI_TYPE & _type, 
	const Vector3 & _position, 
	const Vector3 & _scale, 
	const GAMESTATE & _state)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	UIElement* result = new UIElement(modelMesh, _state, _type);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	UIManager::GetInstance()->addElement(result);
	return result;
}
