#include "UIManager.h"
#include "../Application.h"
#include "../CollisionManager.h"

void UIManager::Init()
{
	//Initialise gamestate
	this->m_gameState = GAMESTATE::GS_MAINMENU;
	GameStateManager::GetInstance()->setState(GS_MAINMENU);

	//Initialise UIElements
	m_cursor = Create::UI("quad", UI_CURSOR, GAMESTATE::GS_MAINMENU, 1, 1, 1, 1, 10, false);

	//MAIN MENU
	Create::UI("main_menu", UI_BACKGROUND, GAMESTATE::GS_MAINMENU, 1, 1, 0, 0, 0);
	Create::UI("start_button", UI_MM_START, GAMESTATE::GS_MAINMENU, 0.15, 0.1, -0.2, 0.11, 1);
	Create::UI("option_button", UI_MM_OPTION, GAMESTATE::GS_MAINMENU, 0.25, 0.1, -0.2, 0, 1);
	Create::UI("quit_button", UI_MM_EXIT, GAMESTATE::GS_MAINMENU, 0.12, 0.1, -0.2, -0.11, 1);
}

void UIManager::Update(double _dt)
{
	//Get the state of the game from the manager
	this->m_gameState = GameStateManager::GetInstance()->getState();

	//Update cursor position
	double x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	float posX = (static_cast<float>(x) - halfWindowWidth);
	float posY = (halfWindowHeight - static_cast<float>(y));

	this->m_cursor->SetPosition(Vector3(posX, posY, 10.f));
	
	//std::cout << "Window Size: " << halfWindowWidth * 2 << ", " << halfWindowHeight * 2 << std::endl;
	//std::cout << "Cursor Pos: " << m_cursor->GetPosition() << std::endl;

	//If mouse clicks, check if its colliding with any buttons
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::BUTTON_TYPE::LMB))
	{
		if(GetElementOnCursor())
			GetElementOnCursor()->Response();
	}

	//Updates the UI
	for (auto &it : m_UIElements)
	{
		if (dynamic_cast<UIElement*>(it)->getState() == m_gameState)
			it->Update(_dt);

		//std::cout << "Button Pos: " << it->GetPosition() << std::endl;
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
	for (auto &it : m_UIElements)
	{
		if (CollisionManager::GetInstance()->CheckPointToAABBCollision(m_cursor->GetPosition(), it, true))
		{
			std::cout << "Detected collision with button" << std::endl;
			return dynamic_cast<UIElement*>(it);
		}
	}
}

