///////////////////////////////////////////////////////////////////////////////
/*!
\file   UIElement.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for UIElement.
*/
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GenericEntity.h"

class UIElement : public GenericEntity
{
public:
	UIElement(Mesh* mesh) : GenericEntity(mesh) 
	{

	};
	~UIElement() {};

	void Response();

private:

};
