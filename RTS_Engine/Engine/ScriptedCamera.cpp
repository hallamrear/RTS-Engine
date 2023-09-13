#include "BennettPCH.h"
#include "ScriptedCamera.h"
#include "InputMonitor.h"

namespace Bennett
{
	ScriptedCamera::ScriptedCamera()
	{
		m_Position = glm::vec3(-10.0f, 5.0f, -10.0f);
	}

	ScriptedCamera::~ScriptedCamera()
	{

	}

	void ScriptedCamera::Update(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;
	}

	glm::mat4 ScriptedCamera::GetViewMatrix()
	{
		return glm::lookAt(m_Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}