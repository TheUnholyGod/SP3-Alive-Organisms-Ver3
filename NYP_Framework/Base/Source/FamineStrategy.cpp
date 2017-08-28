#include "FamineStrategy.h"

FamineStrategy::FamineStrategy()
{
}

FamineStrategy::~FamineStrategy()
{
}

bool FamineStrategy::GetIsDone()
{
	return this->m_isdone;
}

void FamineStrategy::SetParent(FamineBoss * _parent)
{
	this->m_parent = _parent;
}
