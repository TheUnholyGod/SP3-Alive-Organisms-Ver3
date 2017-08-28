#include "HUDManager.h"
#include"../Application.h"


void HUDManager::RenderHUD()
{
	//HEALTH BAR
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * -0.4, Application::GetInstance().GetWindowHeight() * 0.45, 0);
	modelStack.Scale(HPsizeX * 5, 500, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("health_bar"));
	modelStack.PopMatrix();

	//HP
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() *0.0000011, Application::GetInstance().GetWindowHeight() * 0.45, 0);
	modelStack.Scale(50, 50, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("HP"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() *0.0011, Application::GetInstance().GetWindowHeight() * 0.45, 0);
	modelStack.Scale(50, 50, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("bow"));
	modelStack.PopMatrix();


	//WEAPONS
	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("GEO_SPRITE_ANIMATION"));
	modelStack.PopMatrix();*/

	//MINIMAP
	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("GEO_SPRITE_ANIMATION"));
	modelStack.PopMatrix();*/

	//HOTBAR
	//modelStack.PushMatrix();
	//modelStack.Translate(Application::GetInstance().GetWindowWidth() *- 0.2, Application::GetInstance().GetWindowHeight()* -0.00005, 0);
	//modelStack.Scale(180, 450, 1);
	//RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("hot_bar"));
	//modelStack.PopMatrix();

}

void HUDManager::UpdateHUD()
{
	HPsizeX = Player::GetInstance()->GetHealth();
	if (HPsizeX < 0)
		HPsizeX = 1;
	//std::cout << "HPsizeX: "<< HPsizeX << std::endl;

}

HUDManager::HUDManager()
{
}

HUDManager::~HUDManager()
{
}
