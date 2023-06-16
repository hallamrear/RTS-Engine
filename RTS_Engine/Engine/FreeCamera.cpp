#include "BennettPCH.h"
#include "FreeCamera.h"
#include "InputMonitor.h"
#include <glm/matrix.hpp>

namespace Bennett
{
	FreeCamera::FreeCamera()
	{
		m_MouseSensitivity = 10.0f;
	}

	FreeCamera::~FreeCamera()
	{

	}

	glm::mat4 FreeCamera::GetViewMatrix()
	{
		UpdateBasisVectors();
		glm::vec3 target = m_Position + glm::normalize(m_ForwardVector);
		glm::mat4 view = glm::lookAt(m_Position, target, glm::vec3(0.0f, 1.0f, 0.0f));

		return view;
	}

	glm::mat4 FreeCamera::GetProjectionMatrix()
	{
		glm::mat4 projection = glm::perspective(m_FOVAngle, m_AspectRatio, m_NearPlaneDistance, m_FarPlaneDistance);
		//vulkan uses weird coordinates so multiply by -1.
		projection[1][1] *= -1;
		return projection;
	}

	void FreeCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		if (m_InputMonitor->GetKeyState(GLFW_KEY_W)) { Translate(glm::vec3(m_ForwardVector * m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_S)) { Translate(glm::vec3(m_ForwardVector * -m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_A)) { Translate(glm::vec3(m_RightVector * -m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_D)) { Translate(glm::vec3(m_RightVector * m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_SPACE)) { Translate(glm::vec3(m_UpVector * m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_LEFT_SHIFT)) { Translate(glm::vec3(m_UpVector * -m_MovementSpeed * deltaTime)); }

		if (m_IsMouseLocked)
		{
			glm::vec2 mousePos = m_InputMonitor->GetMousePosition();
			glm::vec2 mouseDelta = mousePos - m_MousePositionLastFrame;
			//todo : unhardcode.

			if (mouseDelta != glm::vec2(0.0f, 0.0f))
			{
				Rotate(glm::vec3(mouseDelta.y * -m_MouseSensitivity * m_RotationSpeed * deltaTime, mouseDelta.x * m_MouseSensitivity * m_RotationSpeed * deltaTime, 0.0f));
			}

			m_MousePositionLastFrame = mousePos;
			m_InputMonitor->SetMousePositionToCentre();
		}
		else
		{
			if (m_InputMonitor->GetKeyState(GLFW_KEY_R)) { Rotate(glm::vec3(m_RotationSpeed * deltaTime, 0.0f, 0.0f)); }
			if (m_InputMonitor->GetKeyState(GLFW_KEY_F)) { Rotate(glm::vec3(-m_RotationSpeed * deltaTime, 0.0f, 0.0f)); }
			if (m_InputMonitor->GetKeyState(GLFW_KEY_Q)) { Rotate(glm::vec3(0.0f, -m_RotationSpeed * deltaTime, 0.0f)); }
			if (m_InputMonitor->GetKeyState(GLFW_KEY_E)) { Rotate(glm::vec3(0.0f, m_RotationSpeed * deltaTime, 0.0f)); }
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_TAB))
		{
			m_IsMouseLocked = !m_IsMouseLocked;
			if (m_IsMouseLocked)
			{
				Log("Mouse locked to screen centre.", LOG_SAFE), m_InputMonitor->SetMousePositionToCentre();
				m_MousePositionLastFrame = m_InputMonitor->GetMousePosition();
			}
			else
			{
				Log("Mouse unlocked.", LOG_SAFE);
			}
		};
	}
}