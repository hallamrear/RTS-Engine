#include <GLFW/glfw3.h>
#include "BennettPCH.h"
#include "Application.h"
#include "AssetManager.h"
#include "Model.h"

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

	float r;

	void Application::Update(float DeltaTime)
	{
		r += 5 * DeltaTime;
		if (r ) r -= 360.0f;

		if (!m_ModelOne)
		{
			m_ModelOne = AssetManager::GetModel(m_Renderer, "blasterD");
			m_ModelOne->Position = glm::vec3(2.5f, 0.0f, 0.0f);
		}

		if (!m_ModelTwo)
		{
			m_ModelTwo = AssetManager::GetModel(m_Renderer, "blasterD");
			m_ModelTwo->Position = glm::vec3(-2.5f, 0.0f, 0.0f);
		}

		if (m_ModelOne->Rotation < 0.0f)   m_ModelOne->Rotation += 360.0f;
		if (m_ModelTwo->Rotation > 360.0f) m_ModelTwo->Rotation -= 360.0f;
	}

	void Application::Render()
	{
		m_Renderer.UniformMatrixBuffer.Model = glm::mat4(1.0f); //glm::rotate(glm::mat4(1.0f), glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
		float x = 2.5f * sin(r); float z = 2.5f * cos(r);
		m_Renderer.UniformMatrixBuffer.View = glm::lookAt(glm::vec3(x, 0.0f, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Renderer.UniformMatrixBuffer.Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10.0f);
		m_Renderer.UniformMatrixBuffer.Projection[1][1] *= -1;

		//m_Renderer.Render();
		m_Renderer.StartFrame();
		//Submit
		m_ModelOne->Render(m_Renderer);
		m_ModelTwo->Render(m_Renderer);

		m_Renderer.EndFrame();
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