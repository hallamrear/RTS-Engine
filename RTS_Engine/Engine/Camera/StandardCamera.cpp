#include <BennettPCH.h>
#include <Camera/StandardCamera.h>
#include <System/InputMonitor.h>

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

	void StandardCamera::Update(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		float movementScale = m_MovementSpeed * deltaTime;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_W))
		{
			Translate(glm::vec3(0.0f, 0.0f, movementScale));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_S))
		{
			Translate(glm::vec3(0.0f, 0.0f, -movementScale));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_A))
		{
			Translate(glm::vec3(movementScale, 0.0f, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_D))
		{
			Translate(glm::vec3(-movementScale, 0.0f, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_R))
		{
			Translate(glm::vec3(0.0f, movementScale, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_F))
		{
			Translate(glm::vec3(0.0f, -movementScale, 0.0f));
		}
	}
}