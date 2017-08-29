#ifndef _BASE_DATABASE_H
#define _BASE_DATABASE_H

#include "Database.h"

#include <string>
#include <map>

template <class Key,class Object>
class BaseDatabase : public DataBase
{
protected:
	const std::string k_name, k_filePath;
    std::map<Key, Object*> m_database;
public:
	BaseDatabase(const std::string& name,const std::string& filePath);
	virtual ~BaseDatabase();
	virtual void init();
	virtual void Exit();
	Object* getObject(Key id);
};

#endif
