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
		for (auto& ent : m_Entities)
		{
			ent.Update(DeltaTime);
		}
	}

	void Application::Render()
	{
		m_Renderer.UniformMatrixBuffer.View = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Renderer.UniformMatrixBuffer.Projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10.0f);
		m_Renderer.UniformMatrixBuffer.Projection[1][1] *= -1;

		//m_Renderer.Render();
		m_Renderer.StartFrame();
		//Submit
		for (auto& ent : m_Entities)
		{
			ent.Render(m_Renderer);
		}

		m_Renderer.EndFrame();
	}


	bool Application::Initialise(int argc, char** argv, const WindowDetails& details)
	{
		if (InitialiseWindow(details) && InitialiseRenderer())
		{
			Log("Initialised application successfully.", LOG_SAFE);

			for (size_t i = 0; i < 10; i++)
			{
				float x = rand() % 20 - 10;
				float y = rand() % 20 - 10;

				m_Entities.push_back(Entity());
				m_Entities.back()._Model = AssetManager::GetModel(m_Renderer, "blasterD");
				m_Entities.back().Position = glm::vec3(x, y, 0.0f);
				m_Entities.back().Rotation = rand() % 360 + 1;
			}

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