#include "BennettPCH.h"
#include "StandardCamera.h"
#include "InputMonitor.h"

namespace Bennett
{
	StandardCamera::StandardCamera()
	{
		m_MovementSpeed = 15.0f;
	}

	StandardCamera::~StandardCamera()
	{

	}

	glm::mat4 StandardCamera::GetViewMatrix()
	{
		glm::mat4 view = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		return view;
	}

	glm::mat4 StandardCamera::GetProjectionMatrix()
	{
		glm::mat4 projection = glm::perspective(m_FOVAngle, m_AspectRatio, m_NearPlaneDistance, m_FarPlaneDistance);
		//vulkan uses weird coordinates so multiply by -1.
		projection[1][1] *= -1;
		return projection;
	}

	void StandardCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		float movementScale = m_MovementSpeed * deltaTime;

		if (m_InputMonitor->GetKeyState(VK_W))
		{
			Translate(glm::vec3(0.0f, 0.0f, movementScale));
		}

		if (m_InputMonitor->GetKeyState(VK_S))
		{
			Translate(glm::vec3(0.0f, 0.0f, -movementScale));
		}

		if (m_InputMonitor->GetKeyState(VK_A))
		{
			Translate(glm::vec3(movementScale, 0.0f, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(VK_D))
		{
			Translate(glm::vec3(-movementScale, 0.0f, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(VK_R))
		{
			Translate(glm::vec3(0.0f, movementScale, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(VK_F))
		{
			Translate(glm::vec3(0.0f, -movementScale, 0.0f));
		}
	}
}