#include "BennettPCH.h"
#include "InputMonitor.h"
#include <GLFW/glfw3.h>
#include "Window.h"

namespace Bennett
{
	Window* InputMonitor::m_AttachedWindow = nullptr;
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

	glm::vec2 InputMonitor::GetMousePosition() const
	{
		glm::vec2 position;
		if (!m_IsAttached)
		{
			Log("Tried to get mouse position but InputMonitor has not been attached to a window. Returning 0,0.", LOG_MINIMAL);
		}
		else
		{
			double x = 0.0f;
			double y = 0.0f;
			glfwGetCursorPos(&m_AttachedWindow->GetGLFWWindow(), &x, &y);
			position = glm::vec2(x, y);
		}

		return position;
	}

	void InputMonitor::SetMousePositionToCentre() const
	{
		double x = WINDOW_WIDTH / 2;
		double y = WINDOW_HEIGHT / 2;
		glfwSetCursorPos(&m_AttachedWindow->GetGLFWWindow(), x, y);
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

	void InputMonitor::AttachToWindow(Window& window)
	{
		if (m_IsAttached == false)
		{
			glfwSetKeyCallback(&window.GetGLFWWindow(), InputMonitor::GLFWInputCallback);
			m_AttachedWindow = &window;
			m_IsAttached = true;
		}
	}
}