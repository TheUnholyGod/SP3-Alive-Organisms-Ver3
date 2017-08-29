#include "BaseDatabase.h"
#include "Loader.h"
#include "TileMaker.h"
#include "DatabaseManager.h"

template <class Key, class Object>
inline BaseDatabase<Key, Object>::BaseDatabase(const std::string& name, const std::string& filePath)
    :k_name(name),k_filePath(filePath)
{

}

template <class Key, class Object>
inline BaseDatabase<Key, Object>::~BaseDatabase()
{

}

template <class Key, class Object>
inline void BaseDatabase<Key, Object>::init()
{
	Loader::GetInstance()->LoadData(this->k_filePath);
	std::vector<std::string> data = Loader::GetInstance()->GetData();
	Loader::GetInstance()->ClearData();

	int i = 1;
	for (std::vector<std::string>::iterator it = data.begin();it != data.end();++it)
	{
		this->m_database[i] = new Object(*it);
        ++i;
	}
	DataBaseManager::GetInstance()->AddDatabase(this->k_name,this);
}

template<class Key, class Object>
void BaseDatabase<Key, Object>::Exit()
{
	for (std::map<Key, Object*>::iterator it = this->m_database.begin(); it != this->m_database.end(); ++it)
	{
		delete it->second;
	}
	this->m_database.clear();
}

template <class Key, class Object>
inline Object* BaseDatabase<Key, Object>::getObject(Key id)
{
	return this->m_database[id];
}