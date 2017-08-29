#include "UIManager.h"
#include "../Application.h"
#include "../CollisionManager.h"
#include "Audio/AudioPlayer.h"
#include "SpriteAnimation.h"
#include "MeshList.h"

void UIManager::Init()
{
	//Initialise gamestate
	this->m_gameState = GAMESTATE::GS_MAINMENU;
	GameStateManager::GetInstance()->setState(GS_MAINMENU);

	SpriteAnimation* sa = new SpriteAnimation(*dynamic_cast<SpriteAnimation*>(MeshList::GetInstance()->GetMesh("explosion")));
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(1, 24, 1, 1.0f, true);

		this->explosion = new SpriteEntity(sa);
	}

	
	//Initialise UIElements
	m_cursor = Create::UI("cursor", UI_CURSOR, GAMESTATE::GS_MAINMENU, 1, 1, 1, 1, 2, false);
	m_selecter = Create::UI("selecter", UI_CURSOR, GAMESTATE::GS_MAINMENU, 1, 1, 1, 1, 2, false);

	//Main Menu
	Create::UI("main_menu", UI_BACKGROUND, GAMESTATE::GS_MAINMENU, 1, 1, 0, 0, 0);
	Create::UI("start_button", UI_MM_START, GAMESTATE::GS_MAINMENU, 0.15, 0.1, -0.2, 0.11, 1);
	Create::UI("instruction_button", UI_MM_INTRUCTION, GAMESTATE::GS_MAINMENU, 0.35, 0.1, -0.2, 0, 1);
	Create::UI("quit_button", UI_MM_EXIT, GAMESTATE::GS_MAINMENU, 0.12, 0.1, -0.2, -0.11, 1);

	//Pause Menu
	Create::UI("pause_menu", UI_BACKGROUND, GAMESTATE::GS_PAUSED, 1, 1, 0, 0, 0);
	Create::UI("resume_button", UI_PM_RESUME, GAMESTATE::GS_PAUSED, 0.25, 0.1, 0, 0.11, 1);
	Create::UI("option_button", UI_PM_OPTION, GAMESTATE::GS_PAUSED, 0.30, 0.1, 0, 0, 1);
	Create::UI("quit_button", UI_PM_EXIT_TO_MENU, GAMESTATE::GS_PAUSED, 0.15, 0.1, 0, -0.22, 1);

	//Options Menu
	Create::UI("pause_menu", UI_BACKGROUND, GAMESTATE::GS_OPTIONS, 1, 1, 0, 0, 0);
	Create::UI("quad", UI_OM_VOL_DISPLAY, GAMESTATE::GS_OPTIONS, 0.02, 0.1, 0, 0.0, 1);
	Create::UI("volume_up", UI_OM_VOL_UP, GAMESTATE::GS_OPTIONS, 0.1, 0.1, 0.25, 0, 1);
	Create::UI("volume_button", UI_CURSOR, GAMESTATE::GS_OPTIONS, 0.25, 0.1, 0, 0.2, 1);
	Create::UI("volume_down", UI_OM_VOL_DOWN, GAMESTATE::GS_OPTIONS, 0.1, 0.04, -0.25, 0, 1);
	Create::UI("back_button", UI_OM_EXIT_TO_PAUSE, GAMESTATE::GS_OPTIONS, 0.15, 0.1, 0, -0.22, 1);

	//Instructions
	Create::UI("instruction_menu", UI_BACKGROUND, GAMESTATE::GS_INSTRUCTION, 1, 1, 0, 0, 0);
	Create::UI("back_button", UI_IM_EXIT_TO_MENU, GAMESTATE::GS_INSTRUCTION, 0.25, 0.1, 0, -0.35, 1);

	//Gamover
	Create::UI("gameover_screen", UI_BACKGROUND, GAMESTATE::GS_GAMEOVER, 1, 1, 0, 0, 0);
	Create::UI("restart_button", UI_GO_RESTART, GAMESTATE::GS_GAMEOVER, 0.25, 0.1, 0, -0.33, 1);
	
	//Level complete screen
	Create::UI("castle", UI_BACKGROUND, GAMESTATE::GS_LEVELCOMPLETE, 1, 1, 0, 0, 0);
}

void UIManager::Update(double _dt)
{
	//Get the state of the game from the manager
	this->m_gameState = GameStateManager::GetInstance()->getState();

	if (m_gameState == GS_LEVELCOMPLETE)
	{
		this->explosion->SetPosition(Vector3(Math::RandFloatMinMax(-0.3, 0.3) * Application::GetInstance().GetWindowWidth(), 
			Math::RandFloatMinMax(-0.3, 0.3) * Application::GetInstance().GetWindowHeight(),
			10));
		this->explosion->SetScale(Vector3(200, 200, 200));
		this->explosion->Update(_dt);
		m_explosionTime -= _dt;
		if (m_explosionTime <= 0)
			GameStateManager::GetInstance()->setState(GS_MAINMENU);
	}


	//Update cursor position
	double x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	float posX = (static_cast<float>(x) - halfWindowWidth);
	float posY = (halfWindowHeight - static_cast<float>(y));

	this->m_cursor->SetPosition(Vector3(posX, posY, m_cursor->z_pos));
	
	//std::cout << "Window Size: " << halfWindowWidth * 2 << ", " << halfWindowHeight * 2 << std::endl;
	//std::cout << "Cursor Pos: " << m_cursor->GetPosition() << std::endl;

	//If mouse clicks, check if its colliding with any buttons
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::BUTTON_TYPE::LMB))
	{
		if (GetElementOnCursor())
		{
			GetElementOnCursor()->Response();
		}
	}

	if (GetElementOnCursor() != nullptr)
	{
		if (dynamic_cast<UIElement*>(GetElementOnCursor())->getState() == m_gameState &&
			dynamic_cast<UIElement*>(GetElementOnCursor())->m_type != UI_CURSOR && 
			dynamic_cast<UIElement*>(GetElementOnCursor())->m_type != UI_OM_VOL_DISPLAY)
		{
			m_selecter->posX = GetElementOnCursor()->posX;
			m_selecter->posY = GetElementOnCursor()->posY;
			m_selecter->sizeX = GetElementOnCursor()->sizeX + 0.02;
			m_selecter->sizeY = GetElementOnCursor()->sizeY + 0.02;
			m_selecter->Update(_dt);
		}
	}
	else
	{
		m_selecter->posX = 0;
		m_selecter->posY = 0;
		m_selecter->sizeX = 0;
		m_selecter->sizeY = 0;
		m_selecter->Update(_dt);
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
	m_selecter->Render();
	if(m_gameState == GS_LEVELCOMPLETE)
		explosion->Render();
}

void UIManager::addElement(EntityBase * element)
{
	this->m_UIElements.push_back(element);
}

UIElement * UIManager::GetElementOnCursor()
{
	for (auto &it : m_UIElements)
	{
		if (dynamic_cast<UIElement*>(it)->getState() != m_gameState) continue;
		if (dynamic_cast<UIElement*>(it)->type == UI_CURSOR) continue;
		if (dynamic_cast<UIElement*>(it)->type == UI_BACKGROUND) continue;

		if (CollisionManager::GetInstance()->CheckPointToAABBCollision(m_cursor->GetPosition(), it, true))
		{
			return dynamic_cast<UIElement*>(it);
		}
	}
	return nullptr;
}

