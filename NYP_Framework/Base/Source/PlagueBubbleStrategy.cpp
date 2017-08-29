#include "PlagueBubbleStrategy.h"
#include "PlagueBoss.h"
#include "PoisonGasBubble.h"
#include "GenericEntity.h"

PlagueBubbleStrategy::PlagueBubbleStrategy()
{
}

PlagueBubbleStrategy::~PlagueBubbleStrategy()
{
}

void PlagueBubbleStrategy::Init()
{
	for (int i = 0;i < 3;++i)
	{
		PoisonGasBubbles* bubble = dynamic_cast<PoisonGasBubbles*>(this->m_parent->GetEntity(GenericEntity::PLAGUE_BUBBLE_OBJ));

		if (bubble)
		{
			bubbles.push_back(bubble);
		}
	}
	m_deftotaltimer = 5;
	m_totaltimer = 0;
	m_defactiontimer = 1;
	m_actiontimer = 0;
	action = false;
	m_isdone = false;
}

void PlagueBubbleStrategy::Update(double dt)
{
	m_actiontimer += dt;
	m_totaltimer += dt;
	if (m_actiontimer > m_defactiontimer)
	{
		action = true;
		m_actiontimer -= m_defactiontimer;
	}
	if (m_totaltimer > m_deftotaltimer)
	{
		this->m_isdone = true;
	}
	if (action && bubbles.size())
	{
		PoisonGasBubbles* bubble = bubbles[bubbles.size() - 1];
		float x = Math::RandFloatMinMax(0.5, 2.5);
		float y = Math::RandFloatMinMax(0.5, 2.5);
		float dirx = Math::RandIntMinMax(-1, 1);
		float diry = Math::RandFloatMinMax(-1, 1);
		bubble->SetActive(true);
		bubble->Init(this->m_parent->GetPosition(), Vector3(x * dirx, y * diry), Vector3(), this->m_parent);
        bubble->SetTileID(0);
		bubbles.pop_back();
		this->action = false;
		this->m_actiontimer = 0;
	}
}