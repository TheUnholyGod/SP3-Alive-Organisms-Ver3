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
	for (auto &it : m_UIElements)
	{
		it->RenderUI();
	}
}

void UIManager::addElement(EntityBase * element)
{
	this->m_UIElements.push_back(element);
}

UIElement * UIManager::GetElementOnCursor()
{
	return nullptr;
}

