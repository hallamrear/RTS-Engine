#include "BennettPCH.h"
#include <GLFW/glfw3.h>
#include "InputMonitor.h"

namespace Bennett
{
	bool InputMonitor::m_IsAttached = false;
	std::vector<InputMonitor*> InputMonitor::m_Instances = std::vector<InputMonitor*>();

	void InputMonitor::GLFWInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		for (size_t i = 0; i < m_Instances.size(); i++)
		{
			m_Instances[i]->SetKeyState(key, action != GLFW_RELEASE);
		}
	}

	void InputMonitor::SetKeyState(int key, bool state)
	{
		auto itr = m_TrackedKeys.find(key);

		if (itr != m_TrackedKeys.end())
		{
			itr->second = state;
		}
	}

	bool InputMonitor::GetKeyState(int key)
	{
		auto itr = m_TrackedKeys.find(key);

		if (itr != m_TrackedKeys.end())
		{
			return itr->second;
		}

		return false;
	}

	bool InputMonitor::GetIsEnabled()
	{
		return m_IsEnabled;
	}

	void InputMonitor::SetIsEnabled(const bool& state)
	{
		m_IsEnabled = state;
	}

	InputMonitor::InputMonitor(const std::vector<int>& keyIdsToTrack)
	{
		for (size_t i = 0; i < keyIdsToTrack.size(); i++)
		{
			m_TrackedKeys.insert(std::make_pair(keyIdsToTrack[i], false));
		}

		m_Instances.push_back(this);
	}

	InputMonitor::~InputMonitor()
	{
		m_TrackedKeys.clear();
		m_Instances.erase(std::find(m_Instances.begin(), m_Instances.end(), this));
	}

	void InputMonitor::AttachToWindow(GLFWwindow& window)
	{
		if (m_IsAttached == false)
		{
			glfwSetKeyCallback(&window, InputMonitor::GLFWInputCallback);
			m_IsAttached = true;
		}
	}
}