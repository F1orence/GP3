#include "pch.h"
#include "Transform.h"

glm::mat4 Transform::UpdateTransform()
{
	m_transformMatrix = glm::translate(glm::mat4(1.f), m_position);
	glm::mat4 currentRotate = glm::mat4_cast(m_rotation);
	m_transformMatrix = m_transformMatrix * currentRotate;
	m_transformMatrix = scale(m_transformMatrix, m_scale);
	m_forward = glm::normalize(glm::rotate(m_rotation, glm::vec3(0, 0,-1)));
	m_up = glm::normalize(glm::rotate(m_rotation, glm::vec3(0, 1, 0)));
	m_right = glm::normalize(glm::rotate(m_rotation, glm::vec3(1, 0, 0)));
	m_isDirty = false;
	return m_transformMatrix;
}

Transform::Transform()
{
}

Transform::Transform(glm::vec3 pos) : Transform()
{
	m_position = pos;
}

Transform::Transform(glm::vec3 pos, glm::quat rot) : Transform(pos)
{
	m_rotation = rot;
}

Transform::Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : Transform(pos, rot)
{
	m_scale = scale;
}

void Transform::RotateEulerAxis(float angle, glm::vec3 axis)
{
	axis = glm::normalize(axis);
	glm::quat rot = glm::angleAxis(glm::radians(-angle), axis);
	m_rotation = rot * m_rotation;
	m_isDirty = true;
}

void Transform::LookAt(glm::vec3 target) // https://github.com/opengl-tutorials/ogl/blob/master/common/quaternion_utils.cpp
{
	/*glm::vec3 targetVec = m_position - target;
	targetVec = glm::normalize(targetVec);
	glm::quat rotX = glm::quat(m_forward., targetVec.x); //= glm::angleAxis(glm::radians(10.f), targetVec);
	m_rotation = glm::toQuat(glm::)rot;
	std::cout << targetVec.x << " rot: " << rot.x << std::endl;
	m_isDirty = true;*/
	//-----------------------------------------------------nah----------------------------------------
	
	/*
	glm::vec3 direction = m_position - target;
//	quat LookAt(vec3 direction, vec3 desiredUp) {

		if (length2(direction) < 0.0001f)
			return quat();

		// Recompute desiredUp so that it's perpendicular to the direction
		// You can skip that part if you really want to force desiredUp
		vec3 right = cross(direction, desiredUp);
		desiredUp = cross(right, direction);

		// Find the rotation between the front of the object (that we assume towards +Z,
		// but this depends on your model) and the desired direction
		quat rot1 = RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
		// Because of the 1rst rotation, the up is probably completely screwed up. 
		// Find the rotation between the "up" of the rotated object, and the desired up
		vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
		quat rot2 = RotationBetweenVectors(newUp, desiredUp);

		// Apply them
		return rot2 * rot1; // remember, in reverse order.

		*/

		//---------------------------------------------
		//glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
		//{
			glm::vec3 start = m_forward; //normalize(start);
			glm::vec3 dest = normalize(m_position - target);

			glm::quat final;

			float cosTheta = dot(start, dest);
			glm::vec3 rotationAxis;

			if (cosTheta < -1 + 0.001f) 
			{
				// special case when vectors in opposite directions :
				// there is no "ideal" rotation axis
				// So guess one; any will do as long as it's perpendicular to start
				// This implementation favors a rotation around the Up axis,
				// since it's often what you want to do.
				glm::vec3 rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);

				if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
					rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

				rotationAxis = normalize(rotationAxis);

				final = glm::angleAxis(glm::radians(180.0f), rotationAxis);
			}

			// Implementation from Stan Melax's Game Programming Gems 1 article
			rotationAxis = cross(start, dest);

			float s = sqrt((1 + cosTheta) * 2);
			float invs = 1 / s;

			final = glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);

			//std::cout m_rotation
			m_rotation *= final;
			m_isDirty = true;
		//}
}
