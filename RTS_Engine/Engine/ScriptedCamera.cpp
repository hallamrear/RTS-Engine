#include "BennettPCH.h"
#include "ScriptedCamera.h"
#include "InputMonitor.h"

namespace Bennett
{
	ScriptedCamera::ScriptedCamera()
	{

	}

	ScriptedCamera::~ScriptedCamera()
	{

	}

	glm::mat4 ScriptedCamera::GetViewMatrix()
	{
		return glm::mat4();
	}

	glm::mat4 ScriptedCamera::GetProjectionMatrix()
	{
		return glm::mat4();
	}

	void ScriptedCamera::ProcessInput(const float& deltaTime)
	{
		if (!m_InputMonitor)
			return;
	}
}