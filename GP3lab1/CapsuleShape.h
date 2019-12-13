#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CapsuleShape : public CollisionShape
{

private:
	glm::vec3 m_halfExtents;

public:
	CapsuleShape(float radius, float height); // scale should be 0.1 when radius is 1 and height is 2.5, and the 2:5 ratio should be maintained
};
