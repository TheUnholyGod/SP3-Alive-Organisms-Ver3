#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "minimap.h"
#include "../Source/Manager/HUDManager.h"
#include "EntityManager.h"
class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class Keyboard;

class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	static bool s_ischangelvl;

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	GroundEntity* groundEntity;
	FPSCamera *camera;
	TextEntity* textObj[3];
	Light* lights[2];

	GenericEntity* theCube;
	Keyboard* keyboard;
	static SceneText* sInstance; // The pointer to the object that gets registered
	double m_inputtimer;


	CMinimap* theMinimap;
};

#endif