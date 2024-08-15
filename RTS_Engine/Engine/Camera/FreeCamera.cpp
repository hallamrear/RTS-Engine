#include <BennettPCH.h>
#include <Camera/FreeCamera.h>
#include <glm/matrix.hpp>
#include <Rendering/Window.h>
#include <System/InputMonitor.h>
#include <System/ServiceLocator.h>

namespace Bennett
{
	FreeCamera::FreeCamera() : m_RenderWindow(ServiceLocator::GetWindow())
	{
		m_MouseSensitivity = 0.5f;
		m_MouseDelta = glm::vec2(0.0f, 0.0f);
		m_MovementSpeed = 300.0f;
		m_RotationSpeed = 500.0f;
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

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_HOME)) { m_MovementSpeed++; Log(LOG_SAFE, "Camera movement speed: %f\n", m_MovementSpeed); };
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_END)) { m_MovementSpeed--; Log(LOG_SAFE, "Camera movement speed: %f\n",  m_MovementSpeed); };

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_PRIOR))
		{ 
			m_RotationSpeed++; Log(LOG_SAFE, "Camera rotation speed: %f\n", m_RotationSpeed);
		};

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_NEXT))
		{ 
			m_RotationSpeed--; Log(LOG_SAFE, "Camera rotation speed: %f\n", m_RotationSpeed);
		};

		float scaledMovementSpeed = m_MovementSpeed * deltaTime;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_W))		 { Translate(glm::vec3(m_ForwardVector *  scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_S))	     { Translate(glm::vec3(m_ForwardVector * -scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_A))	     { Translate(glm::vec3(m_RightVector   * -scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_D))	     { Translate(glm::vec3(m_RightVector   *  scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_SPACE))  { Translate(glm::vec3(m_UpVector     *   scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_LSHIFT)) { Translate(glm::vec3(m_UpVector	  *  -scaledMovementSpeed)); }

		if (m_InputMonitor->IsCursorLocked() && IsMouseLookEnabled())
		{
			glm::vec2 mousePos = m_InputMonitor->GetMousePosition();
			m_MouseDelta = mousePos - m_RenderWindow.GetCentre();

			if (m_MouseDelta != glm::vec2(0.0f, 0.0f))
			{
				Rotate(glm::vec3(m_MouseDelta.y * -m_MouseSensitivity * m_RotationSpeed * deltaTime, m_MouseDelta.x * m_MouseSensitivity * m_RotationSpeed * deltaTime, 0.0f));
			}

			m_InputMonitor->SetMousePositionToCentre();
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
			if (m_InputMonitor->IsCursorLocked())
			{
				m_InputMonitor->UnlockCursor(); 
				
				if (ENABLE_LOG_MOUSE_LOCK_MESSAGE)
				{
					Log(LOG_SAFE, "Mouse unlocked.\n");
				}
			}
			else
			{ 
				m_InputMonitor->LockCursor();
				m_InputMonitor->SetMousePositionToCentre();

				if (ENABLE_LOG_MOUSE_LOCK_MESSAGE)
				{
					Log(LOG_SAFE, "Mouse locked to screen centre.\n");
				}
			}
		};
	}
}