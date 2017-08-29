#include "HUDManager.h"
#include"../Application.h"


void HUDManager::RenderHUD()
{
	//HEALTH BAR
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * -0.4 - (100 - HPsizeX), Application::GetInstance().GetWindowHeight() * 0.45, 0);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.4 * HPsizeX/100, Application::GetInstance().GetWindowHeight() * 0.4, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("health_bar"));
	modelStack.PopMatrix();

	//HP
	//modelStack.PushMatrix();
	//modelStack.Translate(Application::GetInstance().GetWindowWidth() *0.0000011, Application::GetInstance().GetWindowHeight() * 0.45, 0);
	//modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.1, Application::GetInstance().GetWindowHeight() * 0.1, 1);
	//RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("HP"));
	//modelStack.PopMatrix();

	//WEAPONS
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() *-0.26, Application::GetInstance().GetWindowHeight()* 0.30, 6);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.1, Application::GetInstance().GetWindowHeight() * 0.1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("bow"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() *-0.43, Application::GetInstance().GetWindowHeight()* 0.30, 6);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.1, Application::GetInstance().GetWindowHeight() * 0.1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("sword"));
	modelStack.PopMatrix();


	//HOTBAR
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() *- 0.345, Application::GetInstance().GetWindowHeight()* 0.30, 5);
	modelStack.Scale(Application::GetInstance().GetWindowWidth() * 0.3, Application::GetInstance().GetWindowHeight() * 0.3, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("hot_bar"));
	modelStack.PopMatrix();

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
