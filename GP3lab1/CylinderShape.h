#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CylinderShape : public CollisionShape
{

private:
	glm::vec3 m_halfExtents;

public:
	CylinderShape(glm::vec3 halfExtents); // scale should be 0.1 when radius is 1 and height is 2.5, and the 2:5 ratio should be maintained
};


