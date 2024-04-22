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
			Log(LOG_MINIMAL, "Existing handle in window class. Destroying...");
			return false;
		}

		HINSTANCE instance = GetModuleHandle(NULL);

		if (details.ClassDetails.WndProcCallback != NULL)
		{
			ATOM win32Class = RegisterWin32Class(instance, details);

			if (!win32Class)
			{
				Log(LOG_SERIOUS, GetLastWin32Error().c_str());
				return false;
			}
		}
		else
		{
			Log(LOG_MINIMAL, "Window Win32 WndProc is NULL, skipping class registration.");
		}

		m_WindowHandle = CreateWin32WindowHandle(instance, details);

		if (!m_WindowHandle)
		{
			Log(LOG_SERIOUS, "Failed to create window.");
			Log(LOG_SERIOUS, GetLastWin32Error().c_str());
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
			Log(LOG_MINIMAL, "Error setting window title - problem with window handle.");
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
			Log(LOG_MINIMAL, "Error setting window title - problem with window handle.");
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
			Log(LOG_MINIMAL, "Error setting window position - problem with window handle.");
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