#include <BennettPCH.h>
#include <Rendering/Window.h>

namespace Bennett
{
	Window::Window()
	{
		m_WaitingToClose = false;
		m_WindowHandle = NULL;		
	}

	Window::~Window()
	{

	}

	bool Window::Initialise(const WindowDetails& details)
	{
		if (m_WindowHandle)
		{
			Log("Existing handle in window class. Destroying...", LOG_MINIMAL);
			return false;
		}

		HINSTANCE instance = GetModuleHandle(NULL);

		if (details.ClassDetails.WndProcCallback != NULL)
		{
			ATOM win32Class = RegisterWin32Class(instance, details);

			if (!win32Class)
			{
				Log(GetLastWin32Error(), LOG_SERIOUS);
				return false;
			}
		}
		else
		{
			Log("Window Win32 WndProc is NULL, skipping class registration.", LOG_MINIMAL);
		}

		m_WindowHandle = CreateWin32WindowHandle(instance, details);

		if (!m_WindowHandle)
		{
			Log("Failed to create window." + GetLastWin32Error(), LOG_SERIOUS);
			return false;
		}

		//todo : implement proper window parenting.
		//m_Parent = details.Parent;
		//details.Parent.AddChild(this);

		return true;
	}

	void Window::SetTitle(const char* title)
	{
		if (m_WindowHandle)
		{
			SetWindowText(m_WindowHandle, title);
		}
		else
		{
			Log("Error setting window title - problem with window handle.", LOG_MINIMAL);
		}
	}

	void Window::SetSize(const glm::vec2& size)
	{
		if (m_WindowHandle)
		{
			SetWindowPos(m_WindowHandle, NULL, 0, 0, (int)size.x, (int)size.y, SWP_NOMOVE);
		}
		else
		{
			Log("Error setting window title - problem with window handle.", LOG_MINIMAL);
		}
	}

	void Window::SetPosition(const glm::vec2& position)
	{
		if (m_WindowHandle)
		{
			SetWindowPos(m_WindowHandle, NULL, (int)position.x, (int)position.y, 0, 0, SWP_NOSIZE);
		}
		else
		{
			Log("Error setting window position - problem with window handle.", LOG_MINIMAL);
		}
	}

	glm::vec2 Window::GetSize() const
	{
		RECT rect{};
		GetClientRect(m_WindowHandle, &rect);
		return glm::vec2(rect.right, rect.bottom);
	}

	glm::vec2 Window::GetPosition() const
	{
		RECT rect{};
		GetClientRect(m_WindowHandle, &rect);
		MapWindowPoints(HWND_DESKTOP, GetParent(m_WindowHandle), (LPPOINT)&rect, 2);
		return glm::vec2(rect.right, rect.bottom);
	}

	const glm::vec2& Window::GetCentre() const
	{
		glm::vec2 halfSize = GetSize();
		halfSize.x /= 2.0f;
		halfSize.y /= 2.0f;
		return halfSize;
	}

	void Window::Close()
	{
		m_WaitingToClose = true;
	}

	const HWND& Window::GetWindowHandle() const
	{
		return m_WindowHandle;
	}

	const bool Window::ShouldClose() const
	{
		return m_WaitingToClose;
	}
}