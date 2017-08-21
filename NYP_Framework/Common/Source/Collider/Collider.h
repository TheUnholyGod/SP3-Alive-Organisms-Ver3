#pragma once
#include "../Vector3.h"
#include "../Mesh.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	// Set the maxAABB and minAABB
	virtual void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	// Set the maxAABB
	virtual void SetMaxAABB(Vector3 maxAABB);
	// Get the maxAABB
	virtual Vector3 GetMaxAABB(void);
	// Set the minAABB
	virtual void SetMinAABB(Vector3 minAABB);
	// Get the minAABB
	virtual Vector3 GetMinAABB(void);
	
	//set radius
	virtual void SetRadius(float _value);
	//get radius
	virtual float GetRadius();

    virtual Vector3 GetSize();
    virtual void SetSize(Vector3 _size);

    virtual void GenerateAABB(Vector3 _pos);

	//check if collider has sphere detection
	bool HasSphere();

	//check if collider has AABB detection
	bool HasAABB();

    void Render();

protected:
	// Vector3 variables to store coordinates of the AABB bounding box of an object
	Vector3 maxAABB;
	Vector3 minAABB;
    Vector3 size;

    Mesh* m_mesh; //For Debugging Purposes

	float radius;

	bool m_bAABB;
	bool m_bSphere;
};
