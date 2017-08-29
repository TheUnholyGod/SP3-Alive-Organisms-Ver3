#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"
#include "../Source/Manager/UIManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "HardwareAbstraction\Keyboard.h"
#include "Particle Effect\ParticleEffect.h"
#include "Audio\AudioPlayer.h"
#include "MapManager.h"
#include "Enemy\Pathing.h"
#include "Enemy\EnemyBase.h"
#include "PlagueBoss.h"
#include "FamineBoss.h"
#include "timer.h"
#include <future>
#include <iostream>

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create entities into the scene
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	Create::Entity("background", Vector3(7 * 5, 7 * 5, -5), Vector3(300, 300));
//	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
//	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	
	/*SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");*/


	// Customise the ground entity
//	groundEntity->SetPosition(Vector3(0, -10, 0));
//	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	//groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	//float fontSize = 25.0f;
	//float halfFontSize = fontSize / 2.0f;
	//for (int i = 0; i < 3; ++i)
	//{
	//	textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	//}
	//textObj[0]->SetText("HELLO WORLD");

	//theEditor = new Editor();

	Player::GetInstance()->Init();
	camera = new FPSCamera();
	Player::GetInstance()->AttachCamera(camera);
	GraphicsManager::GetInstance()->AttachCamera(Player::GetInstance()->getCamera());
	this->keyboard = new Keyboard();
	keyboard->Create(Player::GetInstance());
	Math::InitRNG();

	m_inputtimer = 0;
	MapManager::GetInstance()->Init();
	MapManager::GetInstance()->GenerateBlocks(Player::GetInstance()->GetCurrentLevel());
	MapManager::GetInstance()->GenerateBossBlocks(Player::GetInstance()->GetCurrentLevel());
	Player::GetInstance()->SetPosition(MapManager::GetInstance()->GetAllPlayerStartingPos()[Player::GetInstance()->GetCurrentLevel()]);

	UIManager::GetInstance()->Init();

	//for (int y = 0; y < MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->GetSizeOfLevel(); ++y)
	//{
	//	for (int x = 0; x < MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->GetSizeOfLevel(); ++x)
	//	{
	//		std::cout << MapManager::GetInstance()->GetLevel(Player::GetInstance()->GetCurrentLevel())->GetLevelSetArray()[y][x];
	//	}
	//	std::cout << std::endl;
	//}

	theMinimap = Create::Minimap(false);
	theMinimap->SetBackground(MeshBuilder::GetInstance()->GenerateQuad("minimap", Color(0, 0, 0), 1.f));
	theMinimap->SetBorder(MeshBuilder::GetInstance()->GenerateQuad("minimapborder", Color(0, 0, 1), 1.05f));
	theMinimap->SetAvatar(MeshBuilder::GetInstance()->GenerateQuad("avatar", Color(0.8, 0.8, 0.8), 0.1f));
	theMinimap->SetStencil(MeshBuilder::GetInstance()->GenerateQuad("minimap_stencil", Color(1, 1, 1), 1.0f));
	theMinimap->SetTarget(MeshBuilder::GetInstance()->GenerateQuad("minimap_target", Color(0, 0, 1), 1.f));

	AudioPlayer::GetInstance()->addSound("explosion", "Assets//Sound//explosion.wav"); //Move somewhere to run only once
	AudioPlayer::GetInstance()->addSound("menubgm", "Assets//Sound//menubgm.mp3"); //Move somewhere to run only once
	AudioPlayer::GetInstance()->addSound("gamebgm", "Assets//Sound//gamebgm.mp3"); //Move somewhere to run only once

	AudioPlayer::GetInstance()->StopAllSound();
	AudioPlayer::GetInstance()->playSoundThreaded("menubgm");

   //<REMOVE THIS>
	//Player::GetInstance()->SetIsFightingBoss(true);
	PlagueBoss* pb = new PlagueBoss();
	pb->SetPosition(Vector3(111,102,0));
	pb->Init();
	EntityManager::GetInstance()->AddEntity(pb, true);
	//</REMOVE>
}

void SceneText::Update(double dt)
{
	UIManager::GetInstance()->Update(dt);

	if (GameStateManager::GetInstance()->getState() != GS_PLAYING) return;

	m_inputtimer += dt;
	StopWatch timer2;
	timer2.startTimer();
	Player::GetInstance()->Update(dt);
	//std::cout << "Time for update player: " << timer2.getElapsedTime() << std::endl;
	// Update our entities
	StopWatch timer;
	timer.startTimer();
	EntityManager::GetInstance()->Update(dt);
	theMinimap->Update(dt);
	//std::cout << "Time for update loop: " << timer.getElapsedTime() << std::endl;
	HUDManager::GetInstance()->UpdateHUD();

	keyboard->Read(dt);
	if (m_inputtimer > 0.15f)
	{
		keyboard->Update(dt);
		m_inputtimer -= 0.15f;
	}

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//if (KeyboardController::GetInstance()->IsKeyDown('5'))
	//{
	//	lights[0]->type = Light::LIGHT_POINT;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	//{
	//	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	//{
	//	lights[0]->type = Light::LIGHT_SPOT;
	//}

	//if (KeyboardController::GetInstance()->IsKeyDown('I'))
	//	lights[0]->position.z -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('K'))
	//	lights[0]->position.z += (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('J'))
	//	lights[0]->position.x -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('L'))
	//	lights[0]->position.x += (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('O'))
	//	lights[0]->position.y -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//	lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('P'))
	{
		Create::Enemy(EnemyBase::ENEMY_TYPE::E_MAGGOT, Vector3((int)Player::GetInstance()->GetPosition().x, (int)Player::GetInstance()->GetPosition().y, (int)Player::GetInstance()->GetPosition().z), Vector3(1, 1, 1), true, false, false, 0, Player::GetInstance()->GetIsFightingBoss());
		//Create::Enemy(EnemyBase::ENEMY_TYPE::E_BOMBER, Vector3((int)Player::GetInstance()->GetPosition().x + 1, (int)Player::GetInstance()->GetPosition().y, (int)Player::GetInstance()->GetPosition().z), Vector3(1, 1, 1), true, false, false);
		
		GameStateManager::GetInstance()->setState(GS_PAUSED);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	{
		Create::Particle("particle", Player::GetInstance()->GetPosition(), Vector3(20, 0, 0), EFFECT_TYPE::EFT_FIRE, 0.5, 0.3, Player::GetInstance()->GetIsFightingBoss());
		Create::Enemy(EnemyBase::ENEMY_TYPE::E_FLYING, Vector3((int)Player::GetInstance()->GetPosition().x, (int)Player::GetInstance()->GetPosition().y, (int)Player::GetInstance()->GetPosition().z), Vector3(1, 1, 1), true, false, false, 0, Player::GetInstance()->GetIsFightingBoss());
		
		if (Player::GetInstance()->GetCurrentLevel() == 3)
		{
			GameStateManager::GetInstance()->setState(GS_LEVELCOMPLETE);
			UIManager::GetInstance()->m_explosionTime = 1;
			AudioPlayer::GetInstance()->playSoundThreaded("explosion");
		}
		else
			Player::GetInstance()->StartNextLevel();
	}
	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		//std::cout << "Left Mouse Button was released!" << std::endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		GameStateManager::GetInstance()->setState(GS_GAMEOVER);
		UIManager::GetInstance()->m_explosionTime = 1;
		AudioPlayer::GetInstance()->playSoundThreaded("explosion");
		//std::cout << "Right Mouse Button was released!" << std::endl;
	}
	//if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	//{
	//	//std::cout << "Middle Mouse Button was released!" << std::endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	//{
	//	//std::cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << std::endl;
	//}
	//if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	//{
	//	//std::cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << std::endl;
	//}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs


	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	//GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	//std::ostringstream ss;
	//ss.precision(5);
	//float fps = (float)(1.f / dt);
	//ss << "FPS: " << fps;
	//textObj[1]->SetText(ss.str());
	////std::cout << "FPS: " << fps << std::endl;

	//// Update the player position into textObj[2]
	//std::ostringstream ss1;
	//ss1.precision(4);
	//ss1 << "Player:";
	//textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	if (GameStateManager::GetInstance()->getState() != GS_PLAYING)
	{
		// Setup 2D pipeline then render 2D
		int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
		int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
		GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
		GraphicsManager::GetInstance()->DetachCamera();

		UIManager::GetInstance()->RenderUI();
	}
	else
	{
		// Setup 3D pipeline then render 3D
		GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
		GraphicsManager::GetInstance()->AttachCamera(Player::GetInstance()->getCamera());
		
		EntityManager::GetInstance()->Render();
		Player::GetInstance()->Render();

		// Setup 2D pipeline then render 2D
		int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
		int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
		GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
		GraphicsManager::GetInstance()->DetachCamera();

		theMinimap->RenderUI();
		UIManager::GetInstance()->RenderUI();
		HUDManager::GetInstance()->RenderHUD();
	}
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	Player::Destroy();
	TileMaker::Destroy();
	MeshList::GetInstance()->DeleteAllMesh();
	// Delete the lights
	delete lights[0];
	delete lights[1];
}