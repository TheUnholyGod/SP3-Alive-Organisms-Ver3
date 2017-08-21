///////////////////////////////////////////////////////////////////////////////
/*!
\file   HUDManager.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for singleton HUDManager. Renders the HUD of the game onto the
screen based on GameState. Note: Only responsible for rendering information,
not responsible for updating information.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SingletonTemplate.h"

class HUDManager : public Singleton<HUDManager>
{
	friend Singleton<HUDManager>;
public:
	

private:
	HUDManager() {};
	~HUDManager() {};
};