#ifndef _FLASK_H_
#define _FLASK_H_
#include "Equipment.h"

class Flask :public Equipment
{
public:
	Flask();
	Flask(int level, RUNE_TYPE type);
	virtual ~Flask();

protected:

};
#endif