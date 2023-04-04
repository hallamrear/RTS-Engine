#include <GLFW/glfw3.h>
#include "BennettPCH.h"
#include "Application.h"
#include "Logger.h"

namespace Bennett
{
	bool Application::InitialiseWindow(const WindowDetails& details)
	{
		if (m_Window)
		{
			DestroyWindow();
		}

		//Initialise GLFW.
		glfwInit();

		//Create an empty window.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(details.Width, details.Height, details.Title.c_str(), nullptr, nullptr);

		if (!m_Window)
		{
			Log("Failed to create window.", LOG_CRITICAL);
			return false;
		}
		
		return true;
	}

	void Application::DestroyWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool Application::InitialiseRenderer()
	{
		return m_Renderer.Initialise(*m_Window);
	}

	void Application::DestroyRenderer()
	{
		m_Renderer.Shutdown();
	}

	Application::Application()
	{
		Log("Application created.", LOG_SAFE);
		m_Window = nullptr;
		m_IsRunning = true;
	}

	Application::~Application()
	{
		Log("Destroying application", LOG_MINIMAL);
		Destroy();
	}

	void Application::Update(float DeltaTime)
	{
	}

	void Application::Render()
	{
		m_Renderer.RenderFrame();

		//m_Renderer.Render();

	}


	bool Application::Initialise(int argc, char** argv, const WindowDetails& details)
	{
		if (InitialiseWindow(details) && InitialiseRenderer())
		{
			Log("Initialised application successfully.", LOG_SAFE);
			return true;
		}
		else
		{
			Log("Failed to initialise application.", LOG_CRITICAL);
			return false;
		}
	}

	void Application::Destroy()
	{
		DestroyRenderer();
		DestroyWindow();
	}

	void Application::GameLoop()
	{
		float lTime = glfwGetTime();
		float cTime = glfwGetTime();
		float dTime = glfwGetTime();

		while (m_IsRunning && !glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();

			cTime = glfwGetTime();
			dTime = cTime - lTime;

			if (dTime > 0.1)
				dTime = 0.1f;

			Update(dTime);
			Render();

			lTime = cTime;
		}
	}

	Application* CreateApplication(int argc, char** argv, const WindowDetails& details)
	{
		Application* app = new Application();
		
		if (!app->Initialise(argc, argv, details))
		{
			delete app;
			app = nullptr;
		}

		return app;
	}
}