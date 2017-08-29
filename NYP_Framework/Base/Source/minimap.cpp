#include "Minimap.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "EntityManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "EntityManager.h"
#include "TileEntity.h"
#include "Application.h"
#include "Hitbox.h"
#include "GL\glew.h"

CMinimap::CMinimap()
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
	, m_cMinimap_Avatar(NULL)
	, m_cMinimap_Target(NULL)
	, m_cMinimap_Stencil(NULL)
	, m_iAngle(-90)
	, mode(MODE_2D)
{
	Init();
}
CMinimap::~CMinimap(void)
{
	if (m_cMinimap_Background)
	{
		delete m_cMinimap_Background;
		m_cMinimap_Background = NULL;
	}
	if (m_cMinimap_Border)
	{
		delete m_cMinimap_Border;
		m_cMinimap_Border = NULL;
	}
	if (m_cMinimap_Avatar)
	{
		delete m_cMinimap_Avatar;
		m_cMinimap_Avatar = NULL;
	}
	if (m_cMinimap_Target)
	{
		delete m_cMinimap_Target;
		m_cMinimap_Target = NULL;
	}
}

// Initialise this class instance
bool CMinimap::Init(void)
{
	m_iAngle = 0;

	return true;
}

bool CMinimap::SetTarget(Mesh* aTarget)
{
	if (aTarget != NULL)
	{
		m_cMinimap_Target = aTarget;
		return true;
	}
	return false;
}
Mesh* CMinimap::GetTarget(void) const
{
	return m_cMinimap_Target;
}
// Set the background mesh to this class
bool CMinimap::SetBackground(Mesh* aBackground)
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh* CMinimap::GetBackground(void) const
{
	return m_cMinimap_Background;
}
// Set the Border mesh to this class
bool CMinimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the Border mesh to this class
Mesh* CMinimap::GetBorder(void) const
{
	return m_cMinimap_Border;
}
// Set the Avatar mesh to this class
bool CMinimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_cMinimap_Avatar = anAvatar;
		return true;
	}
	return false;
}
// Get the Avatar mesh to this class
Mesh* CMinimap::GetAvatar(void) const
{
	return m_cMinimap_Avatar;
}

// Set m_iAngle of avatar
bool CMinimap::SetAngle(const int m_iAngle)
{
	this->m_iAngle = m_iAngle;
	return true;
}
// Get m_iAngle
int CMinimap::GetAngle(void) const
{
	return m_iAngle;
}

// Set the Stencil mesh to this class
bool CMinimap::SetStencil(Mesh* aStencil)
{
	if (aStencil != NULL)
	{
		m_cMinimap_Stencil = aStencil;
		return true;
	}
	return false;
}
// Get the Stencil mesh to this class
Mesh* CMinimap::GetStencil(void) const
{
	return m_cMinimap_Stencil;
}

void CMinimap::RenderUI()
{
	if (mode == MODE_3D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
	// Translate the current transformation
	modelStack.Translate(position.x, position.y, position.z);
	// Scale the current transformation
	modelStack.Scale(scale.x, scale.y, scale.z);

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();

	// Enable stencil mode
	glEnable(GL_STENCIL_TEST);

	// Configure stencil mode
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Don't write to colour buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

	if (m_cMinimap_Stencil)
		RenderHelper::RenderMesh(m_cMinimap_Stencil);

	// Switch off stencil function
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Write to colour buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

	modelStack.PushMatrix();
	// Rotate the current transformation
	modelStack.Rotate(m_iAngle, 0.0f, 0.0f, 1.0f);
	if (m_cMinimap_Background)
		RenderHelper::RenderMesh(m_cMinimap_Background);

	// Render enemies
	Vector3 playerPos = Player::GetInstance()->GetPosition();
	for (int i = 0; i < entities.size(); ++i)
	{
		if (!entities[i])
			continue;

		modelStack.PushMatrix();
		TileEntity::BLOCK_TYPE bt = TileEntity::TOTAL;
		if(dynamic_cast<TileEntity*>(entities[i]))
			bt = dynamic_cast<TileEntity*>(entities[i])->block_type;
		switch (bt)
		{
		case TileEntity::SOLID_BLOCK:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("solid_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.1, 0.1, 1);
			break;
		}
		case TileEntity::TOP_PLATFORM:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("platform_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 9.9, 0.001);
			modelStack.Scale(0.1, 0.03, 1);
			break;
		}
		case TileEntity::LADDER:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("ladder_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.1, 0.1, 1);
			break;
		}
		case TileEntity::LADDERWITHPLATFORM:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("platform_ladder_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.1, 0.1, 1);
			break;
		}
		case TileEntity::RUNE_SPAWNER:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("rune_spawner_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.1, 0.1, 1);
			break;
		}
		case TileEntity::BOSS_DOOR:
		{
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("door_block");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.1, 0.2, 1);
			break;
		}
		default:
		{
			if (dynamic_cast<Hitbox*>(entities[i]))break;
			m_cMinimap_Target = MeshList::GetInstance()->GetMesh("redquad");
			modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.001);
			modelStack.Scale(0.05, 0.05, 1);
			break;
		}
		}
		//modelStack.Translate((entities[i]->GetPosition().x - playerPos.x) / 10, (entities[i]->GetPosition().y - playerPos.y) / 10, 0.01);
		//modelStack.Scale(0.1, 0.1, 1);
		RenderHelper::RenderMesh(m_cMinimap_Target);
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// Disable depth test
	glDisable(GL_DEPTH_TEST);

	// Display the Avatar
	if (m_cMinimap_Avatar)
	{
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderHelper::RenderMesh(m_cMinimap_Avatar);
	}
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Disable stencil test
	glDisable(GL_STENCIL_TEST);
	modelStack.PopMatrix();

	if (m_cMinimap_Border)
		RenderHelper::RenderMesh(m_cMinimap_Border);

	modelStack.PopMatrix();
}

void CMinimap::Update(double dt)
{
	position.Set(Application::GetInstance().GetWindowWidth() * 0.35, Application::GetInstance().GetWindowHeight() * 0.3, 0.0f);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.2, Application::GetInstance().GetWindowHeight() * 0.3, 10.0f);
	entities.clear();
	EntityManager::GetInstance()->GetActiveEntity(entities);
}

CMinimap* Create::Minimap(const bool m_bAddToLibrary)
{
	CMinimap* result = CMinimap::GetInstance();
	if (m_bAddToLibrary)
		EntityManager::GetInstance()->AddEntity(result);
	return result;
}
