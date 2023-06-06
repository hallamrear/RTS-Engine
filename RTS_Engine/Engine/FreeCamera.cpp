#include "BennettPCH.h"
#include "FreeCamera.h"
#include "InputMonitor.h"
#include <glm/matrix.hpp>

namespace Bennett
{
	FreeCamera::FreeCamera()
	{

	}

	FreeCamera::~FreeCamera()
	{

	}

	glm::mat4 FreeCamera::GetViewMatrix()
	{	
		glm::quat direction = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
		direction = glm::inverse(Rotation) * direction * Rotation;
		glm::vec3 target = Position + glm::normalize(glm::vec3(direction.x, direction.y, direction.z));
		glm::mat4 view = glm::lookAt(Position, target, glm::vec3(0.0f, 1.0f, 0.0f));
		return view;
	}

	glm::mat4 FreeCamera::GetProjectionMatrix()
	{
		glm::mat4 projection = glm::perspective(FOVAngle, AspectRatio, NearPlaneDistance, FarPlaneDistance);
		//vulkan uses weird coordinates so multiply by -1.
		projection[1][1] *= -1;
		return projection;
	}

	void FreeCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		if (m_InputMonitor->GetKeyState(GLFW_KEY_W)) { Translate(glm::vec3(	0.0f, 0.0f,  MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_S)) { Translate(glm::vec3(	0.0f, 0.0f, -MovementSpeed * deltaTime)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_A)) { Translate(glm::vec3(MovementSpeed * deltaTime, 0.0f,  0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_D)) { Translate(glm::vec3(-MovementSpeed * deltaTime, 0.0f,  0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_SPACE))      { Translate(glm::vec3( 0.0f,  MovementSpeed * deltaTime, 0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_LEFT_SHIFT)) { Translate(glm::vec3( 0.0f, -MovementSpeed * deltaTime, 0.0f)); }

		if (m_InputMonitor->GetKeyState(GLFW_KEY_R)) { Rotate(glm::vec3( RotationSpeed * deltaTime, 0.0f, 0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_F)) { Rotate(glm::vec3(-RotationSpeed * deltaTime, 0.0f, 0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_Q)) { Rotate(glm::vec3(0.0f, -RotationSpeed * deltaTime, 0.0f)); }
		if (m_InputMonitor->GetKeyState(GLFW_KEY_E)) { Rotate(glm::vec3(0.0f,  RotationSpeed * deltaTime, 0.0f)); }
	}
}