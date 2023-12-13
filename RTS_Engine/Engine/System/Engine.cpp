#include <BennettPCH.h>
#include <System/Engine.h>
#include <System/ServiceLocator.h>
#include <System/Manager/AssetManager.h>
#include <System/InputMonitor.h>
#include <System/Manager/LevelManager.h>
#include <System/Manager/FileManagement.h>
#include <Defines/PrimativeModelCreation.h>
#include <Rendering/Model.h>
#include <World/World.h>
#include <Collision/Octree.h>
#include <Defines/DebugDefines.h>

namespace Bennett
{
	Engine* Engine::m_Instance = nullptr;

	Engine::Engine()
	{
		srand((unsigned int)time(NULL));
		Log("Engine created.", LOG_SAFE);
		m_IsRunning = true;
		m_InFocus = true;
		m_EngineControls = nullptr;
		m_Instance = this;
		m_CameraController = nullptr;
	}

	Engine::~Engine()
	{
		Log("Destroying Engine", LOG_MINIMAL);
		Destroy();
	}
	
	void Engine::Update(const float& DeltaTime)
	{
		if (ENABLE_LOG_DELTATIME_AND_FPS)
		{
			Log("DT: " + std::to_string(DeltaTime), LOG_SAFE);
			double fps = 1000.0f / DeltaTime;
			Log("FPS: " + std::to_string(fps), LOG_SAFE);
		}

		m_CameraController->GetCurrentCamera().Update(DeltaTime);
		m_World.Update(DeltaTime);
	}

	void Engine::Render()
	{
		if (!m_InFocus)
			return;

		Renderer& renderer = ServiceLocator::GetRenderer();
		renderer.UniformMatrixBuffer.View = m_CameraController->GetCurrentCamera().GetViewMatrix();
		renderer.UniformMatrixBuffer.Projection = m_CameraController->GetCurrentCamera().GetProjectionMatrix();
		renderer.StartFrame();
		m_World.Render(renderer);
		renderer.EndFrame();
	}

	bool Engine::InitialiseEngineSystems(Window& renderWindow)
	{
		if (!ServiceLocator::Initialise(renderWindow))
		{
			return false;
		}

		m_CameraController = &CameraController::Get();

		InputMonitor::AttachToWindow(ServiceLocator::GetWindow());

		std::vector<int> keys =
		{
			BENNETT_KEY_F1,
			BENNETT_KEY_F2,
			BENNETT_KEY_F3,
			BENNETT_KEY_F4,
			BENNETT_KEY_F5,
			BENNETT_KEY_F6,
			BENNETT_KEY_F7,
			BENNETT_KEY_F8,
			BENNETT_KEY_F9,
			BENNETT_KEY_F10,
			BENNETT_KEY_F11,
			BENNETT_KEY_F12
		};

		m_EngineControls = new Bennett::InputMonitor(keys);

		CreateGenericModels();

		Log("Initialised Engine successfully.", LOG_SAFE);
		return true;
	}

	void Engine::Destroy()
	{
		ServiceLocator::Shutdown();
	}

	void Engine::ProcessInput(const float& DeltaTime)
	{
		if (m_EngineControls)
		{
			if (m_CameraController)
			{
				if (m_EngineControls->GetKeyState(BENNETT_KEY_F1))
				{
					m_CameraController->SetCamera(FREE_CAM);
				}
				
				if (m_EngineControls->GetKeyState(BENNETT_KEY_F2))
				{
					m_CameraController->SetCamera(STANDARD_CAM);
				}

				if (m_EngineControls->GetKeyState(BENNETT_KEY_F3))
				{
					m_CameraController->SetCamera(SCRIPTED_CAMERA);
				}
			}

			if (m_EngineControls->GetKeyState(BENNETT_KEY_F4))
			{
				//TODO : Fix the blocking issue, F4 KEY_UP does not call when the file dialog opens.
				//LevelManager::UnloadLevel(m_World);
				//std::string path = "NO PATH LOADED";
				//bool didYouFindFile = FileManagement::OpenFileDialog(".level", path);
				//if (path != "")
				//{
				//	LevelManager::LoadLevel(path, m_World);
				//}

				LevelManager::UnloadLevel(m_World);
				LevelManager::LoadLevel("TestLevel.level", m_World);
			}

			if (m_EngineControls->GetKeyState(BENNETT_KEY_F9))
			{
				ServiceLocator::GetRenderer().RebuildDefaultShaders();
				ServiceLocator::GetRenderer().RecreateSwapChain();
			}

			if (m_EngineControls->GetKeyState(BENNETT_KEY_F5))
			{
				GetWorld().GetOctree().DecreaseMaxDepth();
			}

			if (m_EngineControls->GetKeyState(BENNETT_KEY_F6))
			{
				GetWorld().GetOctree().IncreaseMaxDepth();
			}
		}
	}

	World& Engine::GetWorld()
	{
		return m_World;
	}

	InputMonitor& Engine::GetEngineControls()
	{
		return *m_EngineControls;
	}

	CameraController& Engine::GetCameraController()
	{
		return *m_CameraController;
	}

	LRESULT CALLBACK Engine::WindowsCallbackProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_NCMOUSEMOVE:
			case WM_NCMOUSELEAVE:
			case WM_NCMOUSEHOVER:
			{
				Engine::SetInFocus(false);
			}
				break;

			case WM_MOUSEMOVE:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MOUSEHOVER:
			case WM_MOUSEWHEEL:
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Engine::SetInFocus(true);
				InputMonitor::Win32InputCallback(message, lParam, wParam);
			}
				break;

			case WM_CLOSE:
			{
				Engine::GetEngineInstance()->SetIsRunning(false);				
			}
				break;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
			}
				break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
				break;

			default:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
				break;
			}
		return 0;
	}

	Engine* Engine::GetEngineInstance()
	{
		return m_Instance;
	};
}