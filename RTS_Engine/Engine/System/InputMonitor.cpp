#include <BennettPCH.h>
#include <Rendering/Window.h>
#include <System/ServiceLocator.h>
#include <System/InputMonitor.h>
#include <System/Engine.h>

namespace Bennett
{
	Window* InputMonitor::m_AttachedWindow = nullptr;
	bool InputMonitor::m_IsAttached = false;
	std::vector<InputMonitor*> InputMonitor::m_Instances = std::vector<InputMonitor*>();

	void InputMonitor::Win32InputCallback(WIN32MSG msg, LPARAM lParam, WPARAM wParam)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_LEFT,    true, false); } break;
		case WM_LBUTTONUP   : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_LEFT,   false, false); }	break;
		case WM_MBUTTONDOWN : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_MIDDLE,  true, false); } break;
		case WM_MBUTTONUP   : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_MIDDLE, false, false); } break;
		case WM_RBUTTONDOWN : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_RIGHT,   true, false); } break;
		case WM_RBUTTONUP   : { InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_RIGHT,  false, false); } break;

		case WM_XBUTTONDOWN: 
		{
			WORD fwButton = GET_XBUTTON_WPARAM(wParam);
			if (fwButton == XBUTTON1)
			{
				InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_XBUTTON1, true, false);
			}
			else if (fwButton == XBUTTON2)
			{
				InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_XBUTTON2, true, false);
			}
			else
			{
				Log("Unrecognised XBUTTON input detection.", LOG_MINIMAL);
			}
		}
			break;

		case WM_XBUTTONUP: 
		{
			WORD fwButton = GET_XBUTTON_WPARAM(wParam);
			if (fwButton == XBUTTON1)
			{
				InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_XBUTTON1, false, false);
			}
			else if (fwButton == XBUTTON2)
			{
				InputMonitor::KeyboardInputCallback(BENNETT_MOUSE_XBUTTON2, false, false);
			}
			else
			{
				Log("Unrecognised XBUTTON input detection.", LOG_MINIMAL);
			}
		} 
			break;

		case WM_MOUSEWHEEL:
		{

		}
			break;

		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
		{
			POINTS p = MAKEPOINTS(lParam);
			glm::vec2 pos = glm::vec2(p.x, p.y);
			InputMonitor::MouseMovementInputCallback(pos);
		}
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			WORD vkCode = LOWORD(wParam);
			WORD keyFlags = HIWORD(lParam);
			BOOL isKeyRepeated = (keyFlags & KF_REPEAT) == KF_REPEAT;
			bool isKeyDown = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;

			//FIX: Left/Right Ctrl and Alt currently do not work.
			if (vkCode == VK_SHIFT || vkCode == VK_CONTROL || vkCode == VK_MENU)
			{
				WORD scanCode = LOBYTE(keyFlags);
				vkCode = LOWORD(MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX));
			}

			InputMonitor::KeyboardInputCallback(vkCode, isKeyDown, isKeyRepeated);
		}
			break;

		default:
		{
			Log("Message got to Win32InputCallback that is not meant to be there.", LOG_MINIMAL);
		}
			break;
		}
	}

	void InputMonitor::KeyboardInputCallback(int vkKey, bool state, bool repeat)
	{
		auto find = Bennett::INPUT_STRINGS.find(vkKey);

		if (ENABLE_LOG_KEYPRESS)
		{
			if (find != INPUT_STRINGS.end())
			{
				std::string str = find->second;
				Log("Key Press: " + str + " - " + std::to_string(state), LOG_SAFE);
			}
		}

		for (size_t i = 0; i < m_Instances.size(); i++)
		{
			m_Instances[i]->SetKeyState(vkKey, state);
		}
	}

	void InputMonitor::MouseMovementInputCallback(const glm::vec2& mousePosition)
	{
		if (ENABLE_LOG_MOUSEMOVEMENT)
		{
			Log("Mouse Pos: " + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y), LOG_SAFE);
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
		if (m_IsCursorLocked == false)
		{
			//TODO : FINISH this rect should define the outline of the render window.
			RECT rect{};
			glm::vec2 size = ServiceLocator::GetWindow().GetSize();
			rect.right = (long)size.x;
			rect.bottom = (long)size.y;
			ClipCursor(&rect);
			m_IsCursorLocked = true;

			while (ShowCursor(FALSE) >= 0);
		}
	}

	void InputMonitor::UnlockCursor()
	{
		ClipCursor(NULL);
		m_IsCursorLocked = false;
		while (ShowCursor(TRUE) <= 0);		
	}

	const bool& InputMonitor::IsCursorLocked() const
	{
		return m_IsCursorLocked;
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
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(m_AttachedWindow->GetWindowHandle(), &p);
			position = glm::vec2(p.x, p.y);
		}

		return position;
	}

	void InputMonitor::SetMousePositionToCentre() const
	{
		glm::vec2 winSize = m_AttachedWindow->GetSize();
		POINT p{};
		//Client coords
		p.x = (long)(winSize.x / 2.0f);
		p.y = (long)(winSize.y / 2.0f);
		ClientToScreen(m_AttachedWindow->GetWindowHandle(), &p);
		//p is now in screen space.
		SetCursorPos(p.x, p.y);
	}

	bool InputMonitor::GetKeyState(int key)
	{
		auto itr = m_TrackedKeys.find(key);

		if (itr != m_TrackedKeys.end())
		{
			return itr->second;
		}

		Log("Tried to get the keystate of a key not being tracked.", LOG_SAFE);

		return false;
	}

	bool InputMonitor::GetIsEnabled() const
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