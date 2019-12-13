#pragma once
#include "Physics.h"

enum CShapeType
{
	BOX, SPHERE, CAPSULE, CYLINDER
};

class CollisionShape
{

protected:
	CShapeType m_type;
	btCollisionShape* m_shape;
	CollisionShape();

public:
	btCollisionShape* Get() { return m_shape; };

};


