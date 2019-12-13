#include "pch.h"
#include "CapsuleShape.h"

CapsuleShape::CapsuleShape(float radius, float height)
{
	btScalar rad = radius;
	btScalar h = height;
	m_shape = new btCapsuleShape(rad, h);
	m_type = CShapeType::CAPSULE;
}