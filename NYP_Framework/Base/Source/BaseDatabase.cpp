#include "BaseDatabase.h"
#include "Loader.h"
#include "TileMaker.h"

template <class Object>
inline BaseDatabase<Object>::BaseDatabase(const std::string& name, const std::string& filePath)
    :k_name(name),k_filePath(filePath)
{

}

template <class Object>
inline BaseDatabase<Object>::~BaseDatabase()
{

}

template <class Object>
inline void BaseDatabase<Object>::init()
{
	Loader::GetInstance()->LoadData(this->k_filePath);
	std::vector<std::string> data = Loader::GetInstance()->GetData();
	int i = 1;
	for (std::vector<std::string>::iterator it = data.begin();it != data.end();++it)
	{
		this->m_database[i] = new Object(*it);
        ++i;
	}
}

template <class Object>
inline Object* BaseDatabase<Object>::getObject(int id)
{
	return this->m_database[id];
}