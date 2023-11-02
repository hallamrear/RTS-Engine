#include <BennettPCH.h>
#include <Camera/FreeCamera.h>
#include <System/InputMonitor.h>
#include <glm/matrix.hpp>

namespace Bennett
{
	FreeCamera::FreeCamera()
	{
		m_MouseSensitivity = 10.0f;
		m_IsMouseLocked = false;
		m_MousePositionLastFrame = m_InputMonitor->GetMousePosition();
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

	void FreeCamera::Update(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_UP))   { m_MovementSpeed++; Log("Camera movement speed: " + std::to_string(m_MovementSpeed), LOG_SAFE); };
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_DOWN)) { m_MovementSpeed--; Log("Camera movement speed: " + std::to_string(m_MovementSpeed), LOG_SAFE); };

		float scaledMovementSpeed = m_MovementSpeed * deltaTime;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_W))		{ Translate(glm::vec3(m_ForwardVector *  scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_S))	    { Translate(glm::vec3(m_ForwardVector * -scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_A))	    { Translate(glm::vec3(m_RightVector   * -scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_D))	    { Translate(glm::vec3(m_RightVector   *  scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_SPACE))  { Translate(glm::vec3(m_UpVector     *  scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_LSHIFT)) { Translate(glm::vec3(m_UpVector	  * -scaledMovementSpeed)); }

		if (m_IsMouseLocked)
		{
			glm::vec2 mousePos = m_InputMonitor->GetMousePosition();
			glm::vec2 mouseDelta = mousePos - m_MousePositionLastFrame;
			m_InputMonitor->SetMousePositionToCentre();

			std::string str = "Delta X: " + std::to_string(mouseDelta.x) + " Y: " + std::to_string(mouseDelta.y);
			Log(str, LOG_SAFE);

			if (mouseDelta != glm::vec2(0.0f, 0.0f))
			{
			//	Rotate(glm::vec3(mouseDelta.y * -m_MouseSensitivity * m_RotationSpeed * deltaTime, mouseDelta.x * m_MouseSensitivity * m_RotationSpeed * deltaTime, 0.0f));
			}

			m_MousePositionLastFrame = mousePos;
		}
		else
		{
			float slowerRotationSpeed = m_RotationSpeed / 2.0f;
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