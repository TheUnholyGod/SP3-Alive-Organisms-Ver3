#ifndef _MESH_LIST_H_
#define _MESH_LIST_H_

#include "SingletonTemplate.h"
#include "Vertex.h"
#include <map>
#include <string>

// Forward Declaration
class Mesh;

using std::string;
using std::map;


class MeshList : public Singleton<MeshList>
{
public:
	friend Singleton<MeshList>;

	Mesh* GetMesh(const string& _meshName);
	void AddMesh(const string& _meshName, Mesh* _newMesh);
	void RemoveMesh(const string& _meshName);
	bool IsMeshEmpty();

private:
	MeshList();
	~MeshList();
	map<string, Mesh*> meshMap;
};

#endif