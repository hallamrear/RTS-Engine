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
		m_Renderer.UniformMatrixBuffer.View = m_Camera.GetViewMatrix();
		m_Renderer.UniformMatrixBuffer.Projection = m_Camera.GetProjectionMatrix();

		//m_Renderer.Render();
		m_Renderer.StartFrame();
		//Submit
		for (auto& ent : m_Entities)
		{
			ent.Render(m_Renderer);
		}

		Floor.Render(m_Renderer);

		m_Renderer.EndFrame();
	}

	bool Application::Initialise(int argc, char** argv, const WindowDetails& details)
	{
		if (InitialiseWindow(details) && InitialiseRenderer())
		{
			Log("Initialised application successfully.", LOG_SAFE);

			for (int i = -10; i < 10; i += 2)
			{
				for (int j = -10; j < 10; j += 2)
				{
					float x = rand() % 20 - 10;
					float y = rand() % 20 - 10;
					float z = 0.0f;
					x = i;
					y = 0.0f;
					z = j;

					m_Entities.push_back(Entity());
					m_Entities.back()._Model = AssetManager::GetModel(m_Renderer, "blasterD");
					m_Entities.back().Position = glm::vec3(x, y, z);
					m_Entities.back().Rotation = glm::quat(glm::vec3(0.0f, rand() % 360 + 1, 0.0f));
				}
			}

			m_Entities.push_back(Entity());
			m_Entities.back()._Model = AssetManager::GetModel(m_Renderer, "car3");
			m_Entities.back().Position = glm::vec3();
			m_Entities.back().Rotation = glm::quat(glm::vec3(0.0f, rand() % 360 + 1, 0.0f));

			Floor._Model = AssetManager::GetModel(m_Renderer, "testFloor");
			Floor.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			Floor.Scale = glm::vec3(10.0f, 10.0f, 10.0f);

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

	void Application::ProcessInput(const float& deltaTime)
	{
		float speed = 5;

		if (glfwGetKey(m_Window, GLFW_KEY_W))
		{
			m_Camera.Translate(glm::vec3(speed * deltaTime, 0.0f, speed * deltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_S))
		{
			m_Camera.Translate(glm::vec3(-speed * deltaTime, 0.0f, -speed * deltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_A))
		{
			m_Camera.Translate(glm::vec3(speed * deltaTime, 0.0f, -speed * deltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_D))
		{
			m_Camera.Translate(glm::vec3(-speed * deltaTime, 0.0f, speed * deltaTime));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_R))
		{
			m_Camera.Translate(glm::vec3(0.0f, speed * deltaTime, 0.0f));
		}

		if (glfwGetKey(m_Window, GLFW_KEY_F))
		{
			m_Camera.Translate(glm::vec3(0.0f, -speed * deltaTime, 0.0f));
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