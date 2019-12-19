#include "pch.h"
#include "CylinderShape.h"

CylinderShape::CylinderShape(glm::vec3 halfExtents)
{
	m_halfExtents = halfExtents;
	m_shape = new btCylinderShape(btVector3(m_halfExtents.x, m_halfExtents.y, m_halfExtents.z));
	m_type = CShapeType::CYLINDER;
}