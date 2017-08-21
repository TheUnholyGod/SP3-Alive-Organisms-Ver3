#include "GameStateManager.h"

GAMESTATE GameStateManager::getState()
{
	return this->m_game_state;
}

void GameStateManager::setState(GAMESTATE state)
{
	this->m_game_state = state;
}
