#include "BennettPCH.h"
#include <GLFW/glfw3.h>
#include "Window.h"

namespace Bennett
{
	Window::Window()
	{
		m_Window = nullptr;
		Initialise();
	}

	Window::Window(const WindowDetails& details)
	{
		m_Window = nullptr;
		Initialise();
		SetWindowDetails(details);
	}

	Window::~Window()
	{
		Destroy();
	}

	void Window::Initialise()
	{
		if (m_Window)
		{
			Destroy();
		}

		//Initialise GLFW.
		glfwInit();

		//Create an empty window.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NO TITLE SET", nullptr, nullptr);

		if (!m_Window)
		{
			Log("Failed to create window.", LOG_CRITICAL);
			Destroy();
		}
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		m_Window = nullptr;
	}

	void Window::SetWindowDetails(const WindowDetails& details)
	{
		SetTitle(details.Title);
		SetSize(glm::vec2(details.Width, details.Height));
	}

	void Window::SetTitle(const std::string& title)
	{
		SetTitle(title.c_str());
	}

	void Window::SetTitle(const char* title)
	{
		if (m_Window)
		{
			glfwSetWindowTitle(m_Window, title);
		}
		else
		{
			Log("Error setting window size.", LOG_MINIMAL);
		}
	}

	void Window::SetSize(const glm::vec2 size)
	{
		if (m_Window)
		{
			glfwSetWindowSize(m_Window, size.x, size.y);
		}
		else
		{
			Log("Error setting window size.", LOG_MINIMAL);
		}
	}

	void Window::Close() const
	{
		glfwSetWindowShouldClose(m_Window, true);
	}

	const bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
}