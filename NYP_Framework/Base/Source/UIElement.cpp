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
#include "Audio\AudioPlayer.h"
#include "PlayerInfo\PlayerInfo.h"

void UIElement::Init()
{
}

void UIElement::Update(double _dt)
{
	if (this->m_type == UI_OM_VOL_DISPLAY)
	{
		this->position = Vector3(Application::GetInstance().GetWindowWidth() * posX, 
			Application::GetInstance().GetWindowHeight() * posY, 
			z_pos);
		this->scale = Vector3(Application::GetInstance().GetWindowWidth() * sizeX * ((float)AudioPlayer::GetInstance()->getCurrentVolume() / 10),
			Application::GetInstance().GetWindowHeight() * sizeY, 
			z_pos);
		this->size = scale;
		this->GenerateAABB(this->position);

		std::cout << AudioPlayer::GetInstance()->getCurrentVolume() << std::endl;
		return;
	}
	else
	{
		this->position = Vector3(Application::GetInstance().GetWindowWidth() * posX, Application::GetInstance().GetWindowHeight() * posY, z_pos);
		this->scale = Vector3(Application::GetInstance().GetWindowWidth() * sizeX, Application::GetInstance().GetWindowHeight() * sizeY, z_pos);
		this->size = scale;
		this->GenerateAABB(this->position);
	}
}

void UIElement::Render()
{
	if (this->m_parent_state == GS_LEVELCOMPLETE)
	{
		switch (Player::GetInstance()->GetCurrentLevel())
		{
		case 0:
			modelMesh = MeshList::GetInstance()->GetMesh("castle");
			break;
		case 1:
			modelMesh = MeshList::GetInstance()->GetMesh("castle1");
			break;
		case 2:
			modelMesh = MeshList::GetInstance()->GetMesh("castle2");
			break;
		case 3:
			modelMesh = MeshList::GetInstance()->GetMesh("castle3");
			break;
		case 4:
			modelMesh = MeshList::GetInstance()->GetMesh("castle4");
			break;
		default:
			modelMesh = MeshList::GetInstance()->GetMesh("castle");
			break;
		}

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderMesh(modelMesh);
		modelStack.PopMatrix();
	}

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
		GameStateManager::GetInstance()->setState(GS_PLAYING);
		AudioPlayer::GetInstance()->StopAllSound();
		AudioPlayer::GetInstance()->playSoundThreaded("gamebgm");
		break;
	}
	case UI_MM_INTRUCTION:
	{
		std::cout << "UI_MM_OPTION" << std::endl;
		GameStateManager::GetInstance()->setState(GS_INSTRUCTION);
		break;
	}
	case UI_MM_EXIT:
	{
		std::cout << "UI_MM_EXIT" << std::endl;
		exit(0);
		break;
	}
	case UI_PM_RESUME:
	{
		std::cout << "UI_PM_RESUME" << std::endl;
		GameStateManager::GetInstance()->setState(GS_PLAYING);
		break;
	}
	case UI_PM_OPTION:
	{
		std::cout << "UI_PM_OPTION" << std::endl;
		GameStateManager::GetInstance()->setState(GS_OPTIONS);
		break;
	}
	case UI_PM_EXIT_TO_MENU:
	{
		std::cout << "UI_PM_EXIT_TO_MENU" << std::endl;
		GameStateManager::GetInstance()->setState(GS_MAINMENU);
		break;
	}
	case UI_OM_VOL_UP:
	{
		std::cout << "UI_OM_VOL_UP" << std::endl;
		AudioPlayer::GetInstance()->increaseVolume(10);
		break;
	}
	case UI_OM_VOL_DOWN:
	{
		std::cout << "UI_OM_VOL_DOWN" << std::endl;
		AudioPlayer::GetInstance()->decreaseVolume(10);
		break;
	}
	case UI_OM_EXIT_TO_PAUSE:
	{
		std::cout << "UI_OM_EXIT_TO_PAUSE" << std::endl;
		GameStateManager::GetInstance()->setState(GS_PAUSED);
		break;
	}
	case UI_IM_EXIT_TO_MENU:
	{
		std::cout << "UI_IM_EXIT_TO_MENU" << std::endl;
		GameStateManager::GetInstance()->setState(GS_MAINMENU);
		break;
	}
	case UI_GO_RESTART:
	{
		std::cout << "UI_GO_RESTART" << std::endl;
		Player::GetInstance()->ResetGame();
		GameStateManager::GetInstance()->setState(GS_PLAYING);
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

UIElement * Create::UI(const std::string& _meshName,
	const UI_TYPE& _type,
	const GAMESTATE& _state,
	const float& _sizeX,
	const float& _sizeY,
	const float& _posX,
	const float& _posY,
	const float& z_val,
	const bool addToManager)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	UIElement* result = new UIElement(modelMesh, _state, _type, _sizeX, _sizeY, _posX, _posY, z_val);
	result->SetPosition(Vector3(0, 0, 0));
	result->SetScale(Vector3(10, 10, 10));
	
	if(!(_type == UI_BACKGROUND || _type == UI_CURSOR))
		result->SetCollider(true);
	else
		result->SetCollider(false);

	if(addToManager)
		UIManager::GetInstance()->addElement(result);
	return result;
}
