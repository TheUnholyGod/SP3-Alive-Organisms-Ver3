#include "DatabaseManager.h"
#include "Database.h"
#include "Loader.h"

DataBaseManager::DataBaseManager()
{
	this->m_db_manager = new std::map<std::string, std::shared_ptr<DataBase>>;
}

DataBaseManager::~DataBaseManager()
{
}

void DataBaseManager::Init()
{
	if(!this->m_db_manager)
		this->m_db_manager = new std::map<std::string, std::shared_ptr<DataBase>>;
}

template<class T, class S>
inline BaseDatabase<T, S>* DataBaseManager::GetDatabase(std::string _key)
{
	if (this->m_db_manager[_key])
		return this->m_db_manager[_key];

	return NULL;
}

bool DataBaseManager::AddDatabase(std::string _key, DataBase* _database)
{
	if (!_database)
		return false;

	this->m_db_manager->insert(std::pair<std::string, std::shared_ptr<DataBase>>(_key, std::shared_ptr<DataBase>(_database)));
	return true;
}
