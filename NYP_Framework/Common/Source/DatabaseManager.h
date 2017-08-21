#ifndef _DATABASEMANAGER_H
#define _DATABASEMANAGER_H

#include "../Source/SingletonTemplate.h"
#include "BaseDatabase.h"

#include <map>
#include <string>
#include <memory>

class DataBase;

class DataBaseManager : public Singleton<DataBaseManager>
{
	friend Singleton<DataBaseManager>;
private:
	std::map<std::string, std::shared_ptr<DataBase>>* m_db_manager;
	const std::string k_filePath;

public:
	DataBaseManager();
	~DataBaseManager();
	void Init();

	template <class T, class S>
	BaseDatabase<T, S>*GetDatabase(std::string _key);

	bool AddDatabase(std::string _key, DataBase* _database);
};

#endif