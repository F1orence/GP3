#include "pch.h"
#include "CameraComp.h"
#include "Camera.h"
#include "Input.h"

void CameraComp::OnUpdate(float deltaTime)
{
	if (INPUT->GetKey(SDLK_w))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetForward());
	}

	if (INPUT->GetKey(SDLK_a))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_d))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_s))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetForward());
	}

	if (INPUT->GetKey(SDLK_SPACE))
	{
		m_entity->GetTransform()->AddPosition(m_entity->GetTransform()->GetUp());
	}

	if (INPUT->GetKey(SDLK_LCTRL))
	{
		m_entity->GetTransform()->AddPosition(-m_entity->GetTransform()->GetUp());
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
