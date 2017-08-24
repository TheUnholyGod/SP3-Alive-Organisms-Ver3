#include "HUDManager.h"

#include"../Application.h"

void HUDManager::RenderHUD()
{
	//HEALTH BAR
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("health_bar"));
	modelStack.PopMatrix();

	//STAMINA BAR
	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("GEO_SPRITE_ANIMATION"));
	modelStack.PopMatrix();*/

	//WEAPONS
	/*modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("GEO_SPRITE_ANIMATION"));
	modelStack.PopMatrix();*/


}

void HUDManager::UpdateHUD()
{

}

HUDManager::HUDManager()
{
}

HUDManager::~HUDManager()
{
}
