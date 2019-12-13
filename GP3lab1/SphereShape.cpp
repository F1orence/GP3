#include "pch.h"
#include "SphereShape.h"

SphereShape::SphereShape(float radius)
{
	m_radius = radius;
	m_shape = new btSphereShape(radius); // scale of standard sphere model should be radius / 10
	m_type = CShapeType::SPHERE;
}