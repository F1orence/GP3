#include "pch.h"
#include "CameraComp.h"
#include "Camera.h"
#include "Input.h"

void CameraComp::OnUpdate(float deltaTime)
{
	if (INPUT->GetKey(SDLK_w))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetForward() * 10.0f * deltaTime);
	}

	if (INPUT->GetKey(SDLK_a))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetRight() * 10.0f * deltaTime);
	}

	if (INPUT->GetKey(SDLK_d))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetRight() * 10.0f * deltaTime);
	}

	if (INPUT->GetKey(SDLK_s))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetForward() * 10.0f * deltaTime);
	}

	if (INPUT->GetKey(SDLK_q))
	{
		m_entity->GetTransform()->RotateEulerAxis(-10 * deltaTime, glm::vec3(0,1,0));
	}

	if (INPUT->GetKey(SDLK_e))
	{
		m_entity->GetTransform()->RotateEulerAxis(10 * deltaTime, glm::vec3(0, 1, 0));
	}

	if (INPUT->GetKey(SDLK_SPACE))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetUp() * 10.0f * deltaTime);
	}

	if (INPUT->GetKey(SDLK_LCTRL))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetUp() * 10.0f * deltaTime);
	}
}

void CameraComp::OnRender()
{
}


void CameraComp::OnAttach()
{
	m_camera = new Camera(m_entity->GetTransform());

	if (Application::GetInstance()->GetCamera() == nullptr)
	{
		Application::GetInstance()->SetCamera(m_camera);
	}
}

glm::mat4 CameraComp::Get()
{
	return m_camera->Get();
}
