#include "BennettPCH.h"
#include "Application.h"
#include "ServiceLocator.h"
#include "AssetManager.h"
#include "InputMonitor.h"
#include "LevelManager.h"
#include "Model.h"
#include "World.h"

using namespace Bennett;

bool Application::InitialiseServices()
{
	return ServiceLocator::Initialise();
}

void Application::ShutdownServices()
{
	ServiceLocator::Shutdown();
}

Application::Application() : m_CameraController(CameraController::Get())
{
	srand(time(NULL));
	Log("Application created.", LOG_SAFE);
	m_IsRunning = true;
	m_ApplicationControls = nullptr;
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
	Renderer& renderer = ServiceLocator::GetRenderer();
	renderer.UniformMatrixBuffer.View = m_CameraController.GetCurrentCamera().GetViewMatrix();
	renderer.UniformMatrixBuffer.Projection = m_CameraController.GetCurrentCamera().GetProjectionMatrix();
	renderer.StartFrame();
	m_World.Render(renderer);
	renderer.EndFrame();
}

bool Application::Initialise(int argc, char** argv, const WindowDetails& details)
{
	if (InitialiseServices())
	{
		ServiceLocator::GetWindow().SetWindowDetails(details);

		Log("Initialised application successfully.", LOG_SAFE);

		std::vector<int> applicationControls =
		{
			GLFW_KEY_F1,
			GLFW_KEY_F2,
			GLFW_KEY_F3,
			GLFW_KEY_F6
		};
		m_ApplicationControls = new InputMonitor(applicationControls);

		InputMonitor::AttachToWindow(ServiceLocator::GetWindow());
		m_CameraController.SetCamera(CAMERA_MODE::FREE_CAM);
		LevelManager::LoadLevel("Assets/testLevel.xml", m_World);
			
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
	ShutdownServices();
}

void Application::GameLoop()
{
	float lTime = glfwGetTime();
	float cTime = lTime;
	float dTime = cTime;

	while (m_IsRunning && !ServiceLocator::GetWindow().ShouldClose())
	{
		glfwPollEvents();

		cTime = (float)glfwGetTime();
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
	m_CameraController.GetCurrentCamera().ProcessInput(DeltaTime);

	if (m_ApplicationControls->GetKeyState(GLFW_KEY_F1))
		m_CameraController.SetCamera(FREE_CAM);
	if (m_ApplicationControls->GetKeyState(GLFW_KEY_F2))
		m_CameraController.SetCamera(STANDARD_CAM);
	if (m_ApplicationControls->GetKeyState(GLFW_KEY_F3))
		m_CameraController.SetCamera(SCRIPTED_CAMERA);

	if (m_ApplicationControls->GetKeyState(GLFW_KEY_F6))
	{
		LevelManager::UnloadLevel(m_World);
		LevelManager::LoadLevel("Assets/testLevel.xml", m_World);
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