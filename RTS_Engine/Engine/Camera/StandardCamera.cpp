#include <BennettPCH.h>
#include <Camera/StandardCamera.h>
#include <System/InputMonitor.h>

namespace Bennett
{
	StandardCamera::StandardCamera()
	{
		m_MovementSpeed = 15.0f;
		SetRotation(glm::vec3(-45.0f, 90.0f, 0.0f));
	}

	StandardCamera::~StandardCamera()
	{

	}

	glm::mat4 StandardCamera::GetViewMatrix()
	{
		UpdateBasisVectors();
		glm::vec3 target = m_Position + glm::normalize(m_ForwardVector);
		glm::mat4 view = glm::lookAt(m_Position, target, glm::vec3(0.0f, 1.0f, 0.0f));
		return view;
	}

	void StandardCamera::Update(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		float scaledMovementSpeed = m_MovementSpeed * deltaTime;

		if (m_InputMonitor->GetKeyState(BENNETT_KEY_W))      { Translate(glm::vec3(BENNETT_FORWARD_VECTOR * scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_S))      { Translate(glm::vec3(BENNETT_BACK_VECTOR    * scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_A))      { Translate(glm::vec3(BENNETT_LEFT_VECTOR    * scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_D))      { Translate(glm::vec3(BENNETT_RIGHT_VECTOR   * scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_SPACE))  { Translate(glm::vec3(BENNETT_UP_VECTOR      * scaledMovementSpeed)); }
		if (m_InputMonitor->GetKeyState(BENNETT_KEY_LSHIFT)) { Translate(glm::vec3(BENNETT_DOWN_VECTOR    * scaledMovementSpeed)); }
	}
}
