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

#include "SingletonTemplate.h"


class UIManager : public Singleton<UIManager>
{
	friend Singleton<UIManager>;
public:
	/*Take in user input and calls respective managers to handle the event*/
	void Update(double _dt);
	/*Renders the current UI based on game state*/
	void RenderUI();

	/*TODO: Add more functions for rendering Menu,Options etc*/
	/*Various game state rendering functions*/
	

private:
	UIManager() {};
	virtual ~UIManager() {};

};