#include "BennettPCH.h"
#include "StandardCamera.h"
#include "InputMonitor.h"

namespace Bennett
{
	StandardCamera::StandardCamera()
	{

	}

	StandardCamera::~StandardCamera()
	{

	}

	glm::mat4 StandardCamera::GetViewMatrix()
	{
		glm::mat4 view = glm::lookAt(Position, Position + glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		return view;
	}

	glm::mat4 StandardCamera::GetProjectionMatrix()
	{
		glm::mat4 projection = glm::perspective(FOVAngle, AspectRatio, NearPlaneDistance, FarPlaneDistance);
		//vulkan uses weird coordinates so multiply by -1.
		projection[1][1] *= -1;
		return projection;
	}

	void StandardCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;

		float speed = 5;

		if (m_InputMonitor->GetKeyState(GLFW_KEY_W))
		{
			Translate(glm::vec3(speed * deltaTime, 0.0f, speed * deltaTime));
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_S))
		{
			Translate(glm::vec3(-speed * deltaTime, 0.0f, -speed * deltaTime));
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_A))
		{
			Translate(glm::vec3(speed * deltaTime, 0.0f, -speed * deltaTime));
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_D))
		{
			Translate(glm::vec3(-speed * deltaTime, 0.0f, speed * deltaTime));
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_R))
		{
			Translate(glm::vec3(0.0f, speed * deltaTime, 0.0f));
		}

		if (m_InputMonitor->GetKeyState(GLFW_KEY_F))
		{
			Translate(glm::vec3(0.0f, -speed * deltaTime, 0.0f));
		}
	}
}