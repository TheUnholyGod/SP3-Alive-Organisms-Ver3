///////////////////////////////////////////////////////////////////////////////
/*!
\file   UIManager.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for singleton UIManager. This class updates and changes the gamestate,
calls the LevelManager to start and pause levels. Also responsible for printing
various game screens: Menu, Options, Gameover etc.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

#include "SingletonTemplate.h"
#include "GameStateManager.h"
#include "MouseController.h"

#include "../UIElement.h"
#include "../SpriteEntity.h"

class GameStateManager;

class UIManager : public Singleton<UIManager>
{
	friend Singleton<UIManager>;
public:
	//Initialise the game state, and all UIElements
	void Init();
	/*Take in user input and calls respective managers to handle the event*/
	void Update(double _dt);
	/*Renders the current UI based on game state*/
	void RenderUI();
	
	//Adds the element into manager
	void addElement(EntityBase* element);

	//Returns the element that is on the mouse
	UIElement* GetElementOnCursor();

	double m_explosionTime = 0;
private:
	UIManager() {};
	virtual ~UIManager() {};

	//Vector to store all UIElements
	std::vector<EntityBase*> m_UIElements;

	SpriteEntity* explosion;

	//State of the game
	GAMESTATE m_gameState;

	UIElement* m_cursor;
	UIElement* m_selecter;
};