#include "ShortSword.h"
#include "Collider\Collider.h"
#include "PlayerInfo\PlayerInfo.h"

ShortSword::ShortSword() : Melee()
{
}

ShortSword::ShortSword(int level, RUNE_TYPE type) : Melee(level, type)
{
    this->m_hitbox = new Collision*;
}

ShortSword::~ShortSword()
{
}

void ShortSword::Init(Player * _owner)
{
    this->m_owner = _owner;
    for (int i = COMBO_BASIC1; i < NUM_COMBO; ++i)
    {
        this->m_hitbox[i] = new Collision;
        this->m_hitbox[i]->SetSize(Vector3(1, 1, 1));
    }
	m_curr_combo = 0;
}

void ShortSword::Action(int _combo)
{
	if (m_curr_combo < COMBO_BASIC1 || m_curr_combo > COMBO_DOWN3)
		return;
	std::cout << "COMBO: " << _combo << std::endl;
	if (_combo >= COMBO_BASIC1 && _combo <= COMBO_BACK3)
		this->m_hitbox[_combo]->GenerateAABB(this->m_owner->GetPosition() + (this->m_owner->GetDirection().x * Vector3(1, 0, 0)));
	else if (_combo >= COMBO_UP1 && _combo <= COMBO_DOWN3)
		this->m_hitbox[_combo]->GenerateAABB(this->m_owner->GetPosition() + (this->m_owner->GetDirection().y * Vector3(0, 1, 0)));
	this->m_curr_combo = _combo;
}

void ShortSword::Update(double dt)
{
}

void ShortSword::Render()
{
    this->m_hitbox[m_curr_combo]->Render();
}