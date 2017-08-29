///////////////////////////////////////////////////////////////////////////////
/*!
\file   UIElement.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for UIElement.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../Source/Manager/GameStateManager.h"
#include "GenericEntity.h"

enum UI_TYPE
{
	UI_CURSOR = 0,
	UI_BACKGROUND,

	UI_MM_START,
	UI_MM_INTRUCTION,
	UI_MM_EXIT,

	UI_PM_RESUME,
	UI_PM_OPTION,
	UI_PM_EXIT_TO_MENU,

	UI_OM_VOL_UP,
	UI_OM_VOL_DOWN,
	UI_OM_VOL_DISPLAY,
	UI_OM_EXIT_TO_PAUSE,

	UI_IM_EXIT_TO_MENU,

	UI_GO_RESTART,

	UI_TOTAL
};

class UIElement : public GenericEntity
{
public:
	UIElement(Mesh* mesh, 
		GAMESTATE state,
		UI_TYPE type, 
		float _sizeX,
		float _sizeY,
		float _posX,
		float _posY,
		float z_val) : GenericEntity(mesh)
	{
		m_parent_state = state;
		m_type = type;

		sizeX = _sizeX;
		sizeY = _sizeY;
		posX = _posX;
		posY = _posY;
		z_pos = z_val;
	};
	~UIElement() {};

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();

	//Do the button's function
	void Response();

	//Returns the parent_state of this button
	GAMESTATE getState();

	float sizeX, sizeY, posX, posY, z_pos;
	GAMESTATE m_parent_state;
	UI_TYPE m_type;

};

namespace Create
{
	UIElement* UI(const std::string& _meshName,
		const UI_TYPE& _type,
		const GAMESTATE& _state,
		const float& _sizeX,
		const float& _sizeY,
		const float& _posX,
		const float& _posY,
		const float& z_val,
		const bool addToManager = true);
};
