#include "UIManager.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

void UIManager::Init()
{
	//Initialise gamestate
	this->m_gameState = GAMESTATE::GS_MAINMENU;

	//Initialise UIElements
}

void UIManager::Update(double _dt)
{
	//Get the state of the game from the manager
	this->m_gameState = GameStateManager::GetInstance()->getState();

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::BUTTON_TYPE::LMB))
	{
		GetElementOnCursor()->Response();
	}
}

void UIManager::RenderUI()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(100, 100, 0);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("quad"));
	modelStack.PopMatrix();

	/*for (auto &it : m_UIElements)
	{
		if(dynamic_cast<UIElement*>(it)->getState() == m_gameState)
			it->RenderUI();
	}*/
}

void UIManager::addElement(EntityBase * element)
{
	this->m_UIElements.push_back(element);
}

UIElement * UIManager::GetElementOnCursor()
{
	return nullptr;
}

