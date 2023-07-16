#include "BennettPCH.h"
#include "InputMonitor.h"
#include "Engine.h"
#include "Window.h"

namespace Bennett
{
	Window* InputMonitor::m_AttachedWindow = nullptr;
	bool InputMonitor::m_IsAttached = false;
	std::vector<InputMonitor*> InputMonitor::m_Instances = std::vector<InputMonitor*>();

	void InputMonitor::Win32InputCallback(int vkKey, bool state, bool repeat)
	{
		for (size_t i = 0; i < m_Instances.size(); i++)
		{
			m_Instances[i]->SetKeyState(vkKey, state != GLFW_RELEASE);
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

	void InputMonitor::LockCursor()
	{

		//TODO : FINISH this rect should define the outline of the render window.
		RECT rect{};
		ClipCursor(&rect);
	}

	void InputMonitor::UnlockCursor()
	{
		ClipCursor(NULL);
	}

	void InputMonitor::ToggleCursorLock()
	{
		if (m_IsCursorLocked)
		{
			UnlockCursor();
		}
		else
		{
			LockCursor();
		}
	}

	glm::vec2 InputMonitor::GetMousePosition() const
	{
		glm::vec2 position = glm::vec2();
		if (!m_IsAttached)
		{
			Log("Tried to get mouse position but InputMonitor has not been attached to a window. Returning 0,0.", LOG_MINIMAL);
		}
		else
		{
			std::string s = "";
			POINT p;
			GetCursorPos(&p);
			s = "SC X " + std::to_string(p.x) + ", Y " + std::to_string(p.y);
			Log(s, LOG_SAFE);
			ScreenToClient(m_AttachedWindow->GetWindowHandle(), &p);
			s = "cc X " + std::to_string(p.x) + ", Y " + std::to_string(p.y) + "\n";
			position = glm::vec2(p.x, p.y);
		}

		return position;
	}

	void InputMonitor::SetMousePositionToCentre() const
	{
		glm::vec2 winSize = m_AttachedWindow->GetSize();
		POINT p;
		p.x = winSize.x / 2.0f;
		p.y = winSize.y / 2.0f;


		SetCursorPos(p.x, p.y);
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
		m_IsCursorLocked = false;
		UnlockCursor();

		m_Instances.push_back(this);
	}

	InputMonitor::~InputMonitor()
	{
		m_TrackedKeys.clear();
		m_Instances.erase(std::find(m_Instances.begin(), m_Instances.end(), this));
	}

	void InputMonitor::AttachToWindow(Window& engine)
	{
		if (m_IsAttached == false)
		{
			m_AttachedWindow = &engine;
			m_IsAttached = true;
		}
	}
}