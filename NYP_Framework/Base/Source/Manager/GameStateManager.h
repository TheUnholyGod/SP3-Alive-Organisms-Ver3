///////////////////////////////////////////////////////////////////////////////
/*!
\file   GameStateManager.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for singleton GameStateManager. Store and updates the current state
of the game.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SingletonTemplate.h"

enum GAMESTATE
{
	GS_MAINMENU = 0,
	GS_PAUSED,
	GS_OPTIONS,
	GS_PLAYING,
	GS_INSTRUCTION,
	GS_LEVELCOMPLETE,
	GS_GAMEOVER,
	GS_LOADING,
	GS_TOTAL
};
	
class GameStateManager : public Singleton<GameStateManager>
{
	friend Singleton<GameStateManager>;
public:

	/*Returns the current state of the game*/
	GAMESTATE getState();

	/*Set the current state of the game*/
	void setState(GAMESTATE state);

private:
	GameStateManager() {};
	~GameStateManager() {};

	//store the state of the game
	GAMESTATE m_game_state;
};