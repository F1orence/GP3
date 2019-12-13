#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CapsuleShape : public CollisionShape
{

private:
	glm::vec3 m_halfExtents;

public:
	CapsuleShape(float radius, float height);
};
