#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "MapManager.h"
#include "MeshBuilder.h"
#include "MeshList.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

//Memory leak checking
//#include <vld.h>

#include "LoadTGA.h"
#include "Utility.h"

#include "SceneText.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::GetInstance().SetWindowHeight(h);
	Application::GetInstance().SetWindowWidth(w);
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "NYP Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();
	InitAllMeshes();

}

void Application::Run()
{
	SceneManager::GetInstance()->SetActiveScene("Start");
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	static float dt = m_timer.getElapsedTime();
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		dt = m_timer.getElapsedTime();
		glfwPollEvents();
		UpdateInput();
		
		//std::cout << "FPS: " << 1/dt << std::endl;
		if (dt > 0.016666666667f)
			dt = 0.016666666667;

		/*StopWatch timer;
		timer.startTimer();*/
		SceneManager::GetInstance()->Update(dt);
		/*std::cout << "Time for update: " << timer.getElapsedTime() << std::endl;*/

		/*StopWatch timer2;
		timer2.startTimer();*/
		SceneManager::GetInstance()->Render();
		/*std::cout << "Time for render: " << timer2.getElapsedTime() << std::endl;*/

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		PostInputUpdate();

        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		
	}
	SceneManager::GetInstance()->Exit();
	EntityManager::GetInstance()->Exit();
	MapManager::GetInstance()->DeleteAllLevels();

	SceneManager::GetInstance()->Destroy();
	EntityManager::GetInstance()->Destroy();
	MapManager::GetInstance()->Destroy();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}

void Application::SetWindowHeight(int h)
{
	this->m_window_height = h;
}

void Application::SetWindowWidth(int w)
{
	this->m_window_width = w;
}

void Application::InitAllMeshes()
{
	if (MeshList::GetInstance()->IsMeshEmpty())
	{
		MeshBuilder::GetInstance()->GenerateAxes("reference");
		MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
		MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("redquad", Color(1, 0, 0), 1.f);
        MeshBuilder::GetInstance()->GenerateQuad("q", Color(0, 1, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("particle", Color(1, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("enemy", Color(1, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuadBackground("background", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("background")->textureID = LoadTGA("Image//background.tga");

		MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
		MeshList::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
		MeshList::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);


		MeshBuilder::GetInstance()->GenerateSpriteAnimation("Idle_Animation", 1, 3 , 1.0f);
		MeshList::GetInstance()->GetMesh("Idle_Animation")->textureID = LoadTGA("Image//idle.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("Running_Animation", 1, 3, 1.0f);
		MeshList::GetInstance()->GetMesh("Running_Animation")->textureID = LoadTGA("Image//running.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("Running_Animationleft", 1, 3, 1.0f);
		MeshList::GetInstance()->GetMesh("Running_Animationleft")->textureID = LoadTGA("Image//runningleft.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("Jumping_Animation", 1, 2, 1.0f);
		MeshList::GetInstance()->GetMesh("Jumping_Animation")->textureID = LoadTGA("Image//jumping.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("Climbing_Animation", 1, 2, 1.0f);
		MeshList::GetInstance()->GetMesh("Climbing_Animation")->textureID = LoadTGA("Image//climbing.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("explosion", 5, 5, 1.0f);
		MeshList::GetInstance()->GetMesh("explosion")->textureID = LoadTGA("Image//explosion.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("enemy_1", 1, 7, 1.0f);
		MeshList::GetInstance()->GetMesh("enemy_1")->textureID = LoadTGA("Image//enemy_1.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("enemy_1_atk", 1, 6, 1.0f);
		MeshList::GetInstance()->GetMesh("enemy_1_atk")->textureID = LoadTGA("Image//enemy_1_atk.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("enemy_2", 1, 4, 1.0f);
		MeshList::GetInstance()->GetMesh("enemy_2")->textureID = LoadTGA("Image//enemy_2.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("maggot", 2, 8, 1.0f);
		MeshList::GetInstance()->GetMesh("maggot")->textureID = LoadTGA("Image//Maggot.tga");

		MeshBuilder::GetInstance()->GenerateQuad("player", Color(1, 0, 0), 0.5f);
	

		//UI
		MeshBuilder::GetInstance()->GenerateQuad("main_menu", 1, 1.f);
		MeshList::GetInstance()->GetMesh("main_menu")->textureID = LoadTGA("Image//menu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("gameover_screen", 1, 1.f);
		MeshList::GetInstance()->GetMesh("gameover_screen")->textureID = LoadTGA("Image//gameover_screen.tga");
		MeshBuilder::GetInstance()->GenerateQuad("instruction_menu", 1, 1.f);
		MeshList::GetInstance()->GetMesh("instruction_menu")->textureID = LoadTGA("Image//instruction_menu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("instruction_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("instruction_button")->textureID = LoadTGA("Image//instruction_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("start_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("start_button")->textureID = LoadTGA("Image//start_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("restart_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("restart_button")->textureID = LoadTGA("Image//restart_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("option_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("option_button")->textureID = LoadTGA("Image//option_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("quit_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("quit_button")->textureID = LoadTGA("Image//quit_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("pause_menu", 1, 1.f);
		MeshList::GetInstance()->GetMesh("pause_menu")->textureID = LoadTGA("Image//pause_menu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("resume_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("resume_button")->textureID = LoadTGA("Image//resume_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("back_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("back_button")->textureID = LoadTGA("Image//back_button.tga");		
		MeshBuilder::GetInstance()->GenerateQuad("volume_button", 1, 1.f);
		MeshList::GetInstance()->GetMesh("volume_button")->textureID = LoadTGA("Image//volume_button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("volume_up", 1, 1.f);
		MeshList::GetInstance()->GetMesh("volume_up")->textureID = LoadTGA("Image//volume_up.tga");		
		MeshBuilder::GetInstance()->GenerateQuad("volume_down", 1, 1.f);
		MeshList::GetInstance()->GetMesh("volume_down")->textureID = LoadTGA("Image//volume_down.tga");
		MeshBuilder::GetInstance()->GenerateQuad("selecter", 1, 1.f);
		MeshList::GetInstance()->GetMesh("selecter")->textureID = LoadTGA("Image//selecter.tga");
		MeshBuilder::GetInstance()->GenerateQuad("cursor", 1, 5.f);
		MeshList::GetInstance()->GetMesh("cursor")->textureID = LoadTGA("Image//cursor.tga");

		MeshBuilder::GetInstance()->GenerateQuad("castle", 1, 1.f);
		MeshList::GetInstance()->GetMesh("castle")->textureID = LoadTGA("Image//castle.tga");

		MeshBuilder::GetInstance()->GenerateQuad("castle1", 1, 1.f);
		MeshList::GetInstance()->GetMesh("castle1")->textureID = LoadTGA("Image//castle_dark_1.tga");

		MeshBuilder::GetInstance()->GenerateQuad("castle2", 1, 1.f);
		MeshList::GetInstance()->GetMesh("castle2")->textureID = LoadTGA("Image//castle_dark_2.tga");

		MeshBuilder::GetInstance()->GenerateQuad("castle3", 1, 1.f);
		MeshList::GetInstance()->GetMesh("castle3")->textureID = LoadTGA("Image//castle_dark_3.tga");

		MeshBuilder::GetInstance()->GenerateQuad("castle4", 1, 1.f);
		MeshList::GetInstance()->GetMesh("castle4")->textureID = LoadTGA("Image//castle_dark_4.tga");

		//Blocks
		MeshBuilder::GetInstance()->GenerateQuad("solid_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("solid_block")->textureID = LoadTGA("Image//block.tga");

		MeshBuilder::GetInstance()->GenerateQuad("solid_block_cracked", 1, 1.f);
		MeshList::GetInstance()->GetMesh("solid_block_cracked")->textureID = LoadTGA("Image//block_cracked.tga");

		MeshBuilder::GetInstance()->GenerateQuad("ladder_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("ladder_block")->textureID = LoadTGA("Image//ladder.tga");

		MeshBuilder::GetInstance()->GenerateQuad("platform_ladder_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("platform_ladder_block")->textureID = LoadTGA("Image//platform_ladder.tga");

		MeshBuilder::GetInstance()->GenerateQuad("platform_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("platform_block")->textureID = LoadTGA("Image//platform.tga");

		MeshBuilder::GetInstance()->GenerateQuad("rune_spawner_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("rune_spawner_block")->textureID = LoadTGA("Image//rune_spawner.tga");

		MeshBuilder::GetInstance()->GenerateQuad("door_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("door_block")->textureID = LoadTGA("Image//door.tga");

		MeshBuilder::GetInstance()->GenerateQuad("exit_door_block", 1, 1.f);
		MeshList::GetInstance()->GetMesh("exit_door_block")->textureID = LoadTGA("Image//door_exit.tga");

		MeshBuilder::GetInstance()->GenerateQuad("arrow_projectile", 1, 1.f);
		MeshList::GetInstance()->GetMesh("arrow_projectile")->textureID = LoadTGA("Image//arrow.tga");

		MeshBuilder::GetInstance()->GenerateQuad("health_bar", 1, 1.f);
		MeshList::GetInstance()->GetMesh("health_bar")->textureID = LoadTGA("Image//healthBar.tga");

		MeshBuilder::GetInstance()->GenerateQuad("hot_bar", 1, 1.f);
		MeshList::GetInstance()->GetMesh("hot_bar")->textureID = LoadTGA("Image//hotbar.tga");

		MeshBuilder::GetInstance()->GenerateQuad("bow", 1, 1.f);
		MeshList::GetInstance()->GetMesh("bow")->textureID = LoadTGA("Image//bow.tga");

		MeshBuilder::GetInstance()->GenerateQuad("sword", 1, 1.f);
		MeshList::GetInstance()->GetMesh("sword")->textureID = LoadTGA("Image//swordpic.tga");


		MeshBuilder::GetInstance()->GenerateQuad("fire_rune_item", 1, 1.f);
		MeshList::GetInstance()->GetMesh("fire_rune_item")->textureID = LoadTGA("Image//fire_rune.tga"); 
		
		MeshBuilder::GetInstance()->GenerateQuad("earth_rune_item", 1, 1.f);	
		MeshList::GetInstance()->GetMesh("earth_rune_item")->textureID = LoadTGA("Image//earth_rune.tga"); 
		
		MeshBuilder::GetInstance()->GenerateQuad("water_rune_item", 1, 1.f);	
		MeshList::GetInstance()->GetMesh("water_rune_item")->textureID = LoadTGA("Image//water_rune.tga"); 
	
		MeshBuilder::GetInstance()->GenerateQuad("wind_rune_item", 1, 1.f);
		MeshList::GetInstance()->GetMesh("wind_rune_item")->textureID = LoadTGA("Image//wind_rune.tga");

		MeshBuilder::GetInstance()->GenerateQuad("normal_rune_item", 1, 1.f);
		MeshList::GetInstance()->GetMesh("normal_rune_item")->textureID = LoadTGA("Image//normal_rune.tga");

		MeshBuilder::GetInstance()->GenerateQuad("bubble_projectile", 1, 1.f);
		MeshList::GetInstance()->GetMesh("bubble_projectile")->textureID = LoadTGA("Image//bubble.tga");

		MeshBuilder::GetInstance()->GenerateQuad("bubble_gas", 1, 1.f);
		MeshList::GetInstance()->GetMesh("bubble_gas")->textureID = LoadTGA("Image//gas_bubble.tga");

        MeshBuilder::GetInstance()->GenerateSpriteAnimation("plagueboss", 4, 6, 0);
        MeshList::GetInstance()->GetMesh("plagueboss")->textureID = LoadTGA("Image//Plague.tga");

		MeshBuilder::GetInstance()->GenerateSpriteAnimation("famineboss", 3, 3, 0);
		MeshList::GetInstance()->GetMesh("famineboss")->textureID = LoadTGA("Image//famine.tga");


		MeshBuilder::GetInstance()->GenerateQuad("att", 1, 1.f);
		MeshList::GetInstance()->GetMesh("att")->textureID = LoadTGA("Image//hitbox.tga");
		/*MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
		MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");

		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");*/
	}
}