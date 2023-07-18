#include "BennettPCH.h"
#include "Window.h"

namespace Bennett
{
	Window::Window()
	{
		m_WaitingToClose = false;
		m_WindowHandle = NULL;
		
	}

	Window::~Window()
	{
		Destroy();
	}

	bool Window::Initialise(const WindowDetails& details)
	{
		if (m_WindowHandle)
		{
			Log("Existing handle in window class. Destroying...", LOG_MINIMAL);
			Destroy();
		}

		HINSTANCE instance = GetModuleHandle(NULL);

		ATOM win32Class = RegisterWin32Class(instance, details);

		if (!win32Class)
		{
			Log(GetLastWin32Error(), LOG_SERIOUS);
			return false;
		}

		m_WindowHandle = CreateWin32WindowHandle(instance, details);

		if (!m_WindowHandle)
		{
			Log("Failed to create window." + GetLastWin32Error(), LOG_SERIOUS);
			Destroy();
			return false;
		}

		return true;
	}

	void Window::Destroy()
	{
		BOOL result = DestroyWindow(m_WindowHandle);

		if (result == 0)
		{
			Log(GetLastWin32Error(), LOG_SERIOUS);
		}

		m_WaitingToClose = false;
		m_WindowHandle = nullptr;
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
			SetWindowPos(m_WindowHandle, NULL, 0, 0, size.x, size.y, SWP_NOMOVE);
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
			SetWindowPos(m_WindowHandle, NULL, position.x, position.y, 0, 0, SWP_NOSIZE);
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