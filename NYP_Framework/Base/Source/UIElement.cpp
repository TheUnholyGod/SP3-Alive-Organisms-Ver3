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
#include "Application.h"

void UIElement::Init()
{
}

void UIElement::Update(double _dt)
{
	if (this->type == UI_BACKGROUND)
	{
		this->scale = Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0);
		this->size = scale;
	}
	this->GenerateAABB(this->position);
}

void UIElement::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void UIElement::Response()
{
	std::cout << "Responding with..." << std::endl;
	switch (m_type)
	{
	case UI_MM_START:
	{
		std::cout << "UI_MM_START" << std::endl;
		break;
	}
	case UI_MM_OPTION:
	{
		std::cout << "UI_MM_OPTION" << std::endl;
		break;
	}
	case UI_MM_EXIT:
	{
		std::cout << "UI_MM_EXIT" << std::endl;
		break;
	}
	case UI_PM_RESUME:
	{
		std::cout << "UI_PM_RESUME" << std::endl;
		break;
	}
	case UI_PM_OPTION:
	{
		std::cout << "UI_PM_OPTION" << std::endl;
		break;
	}
	case UI_PM_EXIT_TO_MENU:
	{
		std::cout << "UI_PM_EXIT_TO_MENU" << std::endl;
		break;
	}
	case UI_OM_VOL_UP:
	{
		std::cout << "UI_OM_VOL_UP" << std::endl;
		break;
	}
	case UI_OM_VOL_DOWN:
	{
		std::cout << "UI_OM_VOL_DOWN" << std::endl;
		break;
	}
	case UI_OM_EXIT_TO_PAUSE:
	{
		std::cout << "UI_OM_EXIT_TO_PAUSE" << std::endl;
		break;
	}
	default:
		std::cout << "Nothing!" << std::endl;
		break;
	}
}

GAMESTATE UIElement::getState()
{
	return this->m_parent_state;
}

UIElement * Create::UI(const std::string & _meshName, 
	const UI_TYPE & _type, 
	const Vector3 & _position, 
	const Vector3 & _scale, 
	const GAMESTATE & _state,
	const bool addToManager)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	UIElement* result = new UIElement(modelMesh, _state, _type);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->SetSize(_scale);

	if(addToManager)
		UIManager::GetInstance()->addElement(result);
	return result;
}