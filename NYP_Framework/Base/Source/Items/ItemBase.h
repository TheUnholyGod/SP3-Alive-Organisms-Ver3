///////////////////////////////////////////////////////////////////////////////
/*!
\file   ItemBase.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for ItemBase class. Stores ItemBase params.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include "Loader.h"

class ItemBase
{
public:
	ItemBase()
		: m_ID(0),
		m_name(""),
		m_description(""),
		m_textureLocation("") {};
	~ItemBase() {};

protected:
	int m_ID;
	std::string m_name;
	std::string m_description;
	std::string m_textureLocation;
	static const int sk_max_level = 5;
	int m_curr_max_level;
};

