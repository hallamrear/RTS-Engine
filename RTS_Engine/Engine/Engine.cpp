#include "BennettPCH.h"
#include "Engine.h"
#include "ServiceLocator.h"
#include "TerrainChunk.h"
#include "AssetManager.h"
#include "InputMonitor.h"
#include "LevelManager.h"
#include "Model.h"
#include "World.h"

namespace Bennett
	{
	Engine::Engine() : m_CameraController(CameraController::Get())
	{
		srand(time(NULL));
		Log("Engine created.", LOG_SAFE);
		m_IsRunning = true;
		m_EngineControls = nullptr;
	}

	Engine::~Engine()
	{
		Log("Destroying Engine", LOG_MINIMAL);
		Destroy();
	}
	
	void Engine::Update(float DeltaTime)
	{
		m_World.Update(DeltaTime);
	}

	void Engine::Render()
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		renderer.UniformMatrixBuffer.View = m_CameraController.GetCurrentCamera().GetViewMatrix();
		renderer.UniformMatrixBuffer.Projection = m_CameraController.GetCurrentCamera().GetProjectionMatrix();
		renderer.StartFrame();
		m_World.Render(renderer);
		renderer.EndFrame();
	}

	bool Engine::Initialise(Window& renderWindow)
	{
		ServiceLocator::Initialise(renderWindow);

		Log("Initialised Engine successfully.", LOG_SAFE);

		std::vector<int> EngineControls =
		{
			GLFW_KEY_F1,
			GLFW_KEY_F2,
			GLFW_KEY_F3,
			GLFW_KEY_F4,
			GLFW_KEY_F5,
			GLFW_KEY_F6,
			GLFW_KEY_F7,
			GLFW_KEY_F8,
			GLFW_KEY_F9,
			GLFW_KEY_F10,
			GLFW_KEY_F11,
			GLFW_KEY_F12

		};
		m_EngineControls = new InputMonitor(EngineControls);

		InputMonitor::AttachToWindow(ServiceLocator::GetWindow());
		m_CameraController.SetCamera(CAMERA_MODE::FREE_CAM);
		LevelManager::LoadLevel(ServiceLocator::GetResourceFolderLocation() + "testLevel.xml", m_World);

		return true;
	}

	void Engine::Destroy()
	{
		ServiceLocator::Shutdown();
	}

	void Engine::ProcessInput(const float& DeltaTime)
	{
		m_CameraController.GetCurrentCamera().ProcessInput(DeltaTime);

		if (m_EngineControls->GetKeyState(GLFW_KEY_F1))
			m_CameraController.SetCamera(FREE_CAM);
		if (m_EngineControls->GetKeyState(GLFW_KEY_F2))
			m_CameraController.SetCamera(STANDARD_CAM);
		if (m_EngineControls->GetKeyState(GLFW_KEY_F3))
			m_CameraController.SetCamera(SCRIPTED_CAMERA);

		if (m_EngineControls->GetKeyState(GLFW_KEY_F4))
		{
			LevelManager::UnloadLevel(m_World);
			LevelManager::LoadLevel("Assets/testLevel.xml", m_World);
		}
	
		if (m_EngineControls->GetKeyState(GLFW_KEY_F9))
		{
			ServiceLocator::GetRenderer().RebuildDefaultShaders();
		}

	
	}

	World& Engine::GetWorld()
	{
		return m_World;
	}
}