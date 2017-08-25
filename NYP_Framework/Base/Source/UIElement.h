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
	UI_CURSOR,
	UI_BACKGROUND,

	UI_MM_START,
	UI_MM_OPTION,
	UI_MM_EXIT,

	UI_PM_RESUME,
	UI_PM_OPTION,
	UI_PM_EXIT_TO_MENU,

	UI_OM_VOL_UP,
	UI_OM_VOL_DOWN,
	UI_OM_EXIT_TO_PAUSE,

	UI_TOTAL
};

class UIElement : public GenericEntity
{
public:
	UIElement(Mesh* mesh, 
		GAMESTATE state,
		UI_TYPE type) : GenericEntity(mesh) 
	{
		m_parent_state = state;
		m_type = type;
	};
	~UIElement() {};

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();

	//Do the button's function
	void Response();

	//Returns the parent_state of this button
	GAMESTATE getState();

private:
	GAMESTATE m_parent_state;
	UI_TYPE m_type;
};

namespace Create
{
	UIElement* UI(const std::string& _meshName,
		const UI_TYPE& _type,
		const Vector3& _position,
		const Vector3& _scale,
		const GAMESTATE& _state,
		const bool addToManager = true);
};
