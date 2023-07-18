#include "BennettPCH.h"
#include "FreeCamera.h"
#include "InputMonitor.h"
#include <glm/matrix.hpp>

namespace Bennett
{
	FreeCamera::FreeCamera()
	{
		m_MouseSensitivity = 10.0f;
		m_MovementSpeed = 50.0f;
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

	void FreeCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_W))		{ Translate(glm::vec3(m_ForwardVector *  m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_S))	    { Translate(glm::vec3(m_ForwardVector * -m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_A))	    { Translate(glm::vec3(m_RightVector   * -m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_D))	    { Translate(glm::vec3(m_RightVector   *  m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_SPACE))  { Translate(glm::vec3(m_UpVector      *  m_MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_LSHIFT)) { Translate(glm::vec3(m_UpVector	  * -m_MovementSpeed * deltaTime)); }

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
			float slowerRotationSpeed = m_RotationSpeed / 4.0f;
			if (m_InputMonitor->GetKeyState(BENNETT_KEY_R)) { Rotate(glm::vec3( slowerRotationSpeed * deltaTime, 0.0f, 0.0f)); }
			if (m_InputMonitor->GetKeyState(BENNETT_KEY_F)) { Rotate(glm::vec3(-slowerRotationSpeed * deltaTime, 0.0f, 0.0f)); }
			if (m_InputMonitor->GetKeyState(BENNETT_KEY_Q)) { Rotate(glm::vec3(0.0f, -slowerRotationSpeed * deltaTime, 0.0f)); }
			if (m_InputMonitor->GetKeyState(BENNETT_KEY_E)) { Rotate(glm::vec3(0.0f,  slowerRotationSpeed * deltaTime, 0.0f)); }
		}

		if (m_InputMonitor->GetKeyState(VK_TAB))
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