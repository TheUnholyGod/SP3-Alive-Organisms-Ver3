#include "PlagueStrategy.h"

PlagueStrategy::PlagueStrategy()
{
}

PlagueStrategy::~PlagueStrategy()
{
}

bool PlagueStrategy::GetIsDone()
{
	return this->m_isdone;
}

void PlagueStrategy::SetParent(PlagueBoss * _parent)
{
	this->m_parent = _parent;
}
