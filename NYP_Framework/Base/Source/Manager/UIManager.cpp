#include "UIManager.h"
#include "../Application.h"

void UIManager::Init()
{
	//Initialise gamestate
	this->m_gameState = GAMESTATE::GS_MAINMENU;

	//Initialise UIElements
	m_cursor = Create::UI("quad", UI_CURSOR, Vector3(0, 0, 0), Vector3(10, 10, 10), GAMESTATE::GS_MAINMENU, false);
}

void UIManager::Update(double _dt)
{
	//Get the state of the game from the manager
	this->m_gameState = GameStateManager::GetInstance()->getState();

	//Update cursor position
	double x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float posX = (static_cast<float>(x) - halfWindowWidth);
	float posY = (halfWindowHeight - static_cast<float>(y));

	std::cout << "Window Size: " << halfWindowWidth * 2 << ", " << halfWindowHeight * 2 << std::endl;
	this->m_cursor->SetPosition(Vector3(posX, posY, 10.f));


	//If mouse clicks, check if its colliding with any buttons
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::BUTTON_TYPE::LMB))
	{
		if(GetElementOnCursor())
			GetElementOnCursor()->Response();
	}
}

void UIManager::RenderUI()
{
	for (auto &it : m_UIElements)
	{
		if(dynamic_cast<UIElement*>(it)->getState() == m_gameState)
			it->Render();
	}

	m_cursor->Render();
}

void UIManager::addElement(EntityBase * element)
{
	this->m_UIElements.push_back(element);
}

UIElement * UIManager::GetElementOnCursor()
{
	return nullptr;
}

