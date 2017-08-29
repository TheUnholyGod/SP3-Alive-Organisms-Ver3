#include "MeshList.h"
#include "Mesh.h"

void MeshList::DeleteAllMesh()
{
	for (map<string, Mesh*>::iterator it = meshMap.begin(); it != meshMap.end(); ++it)
	{
		Mesh* m = it->second;
		if(m != nullptr)
			delete m;
		m = nullptr;
	}
	meshMap.clear();
}

Mesh* MeshList::GetMesh(const string& _meshName)
{
	if (meshMap.count(_meshName) != 0)
		return meshMap[_meshName];

	return nullptr;
}

void MeshList::AddMesh(const string& _meshName, Mesh* _newMesh)
{
	// Trivial Rejection : Invalid pointer provided
	if (_newMesh == nullptr)
		return;

	// Clean up first if there is an existing mesh with the same name
	RemoveMesh(_meshName);

	// Add the mesh now
	meshMap[_meshName] = _newMesh;
}

void MeshList::RemoveMesh(const string& _meshName)
{
	Mesh* currMesh = GetMesh(_meshName);
	if (currMesh != nullptr)
	{
		delete currMesh;
		meshMap.erase(_meshName);
	}
}

bool MeshList::IsMeshEmpty()
{
	if (meshMap.size() > 0)
		return false;

	return true;
}

MeshList::MeshList()
{
}

MeshList::~MeshList()
{
	for (std::map<std::string, Mesh*>::iterator it = meshMap.begin(); it != meshMap.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	meshMap.clear();
}
