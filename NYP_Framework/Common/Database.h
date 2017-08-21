#ifndef _DATABASE_H
#define _DATABASE_H

class DataBase
{
public:
	virtual ~DataBase();
	virtual void init() = 0;

};

#endif