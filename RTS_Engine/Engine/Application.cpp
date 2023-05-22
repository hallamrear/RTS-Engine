#include <GLFW/glfw3.h>
#include "BennettPCH.h"
#include "Application.h"
#include "AssetManager.h"
#include "LevelManager.h"
#include "Model.h"
#include "World.h"

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
		srand(time(NULL));
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
		m_World.Update(DeltaTime);
	}

	void Application::Render()
	{
		m_Renderer.UniformMatrixBuffer.View = m_Camera.GetViewMatrix();
		m_Renderer.UniformMatrixBuffer.Projection = m_Camera.GetProjectionMatrix();

		//m_Renderer.Render();
		m_Renderer.StartFrame();
		
		m_World.Render(m_Renderer);

		m_Renderer.EndFrame();
	}

	bool Application::Initialise(int argc, char** argv, const WindowDetails& details)
	{
		if (InitialiseWindow(details) && InitialiseRenderer())
		{
			Log("Initialised application successfully.", LOG_SAFE);

			LevelManager::LoadLevel(m_Renderer, "Assets/testLevel.xml", m_World);

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

			if (dTime > TIMESTEP_CAP)
				dTime = TIMESTEP_CAP;

			ProcessInput(dTime);
			Update(dTime);
			Render();

			lTime = cTime;
		}
	}

	void Application::ProcessInput(const float& DeltaTime)
	{
		float speed = 5;

		if (glfwGetKey(m_Window, GLFW_KEY_W))
		{
			m_Camera.Translate(glm::vec3(speed * DeltaTime, 0.0f, speed * DeltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_S))
		{
			m_Camera.Translate(glm::vec3(-speed * DeltaTime, 0.0f, -speed * DeltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_A))
		{
			m_Camera.Translate(glm::vec3(speed * DeltaTime, 0.0f, -speed * DeltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_D))
		{
			m_Camera.Translate(glm::vec3(-speed * DeltaTime, 0.0f, speed * DeltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_R))
		{
			m_Camera.Translate(glm::vec3(0.0f, speed * DeltaTime, 0.0f));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_F))
		{
			m_Camera.Translate(glm::vec3(0.0f, -speed * DeltaTime, 0.0f));
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