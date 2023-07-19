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
	Engine* Engine::m_Instance = nullptr;

	Engine::Engine() : m_CameraController(CameraController::Get())
	{
		srand(time(NULL));
		Log("Engine created.", LOG_SAFE);
		m_IsRunning = true;
		m_InFocus = true;
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
		if (!m_InFocus)
			return;

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
		m_EngineControls = new InputMonitor(EngineControls);

		InputMonitor::AttachToWindow(ServiceLocator::GetWindow());
		m_CameraController.SetCamera(CAMERA_MODE::FREE_CAM);
		m_CameraController.GetCurrentCamera().SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
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

		if (m_EngineControls->GetKeyState(BENNETT_KEY_F1))
			m_CameraController.SetCamera(FREE_CAM);
		if (m_EngineControls->GetKeyState(BENNETT_KEY_F2))
			m_CameraController.SetCamera(STANDARD_CAM);
		if (m_EngineControls->GetKeyState(BENNETT_KEY_F3))
			m_CameraController.SetCamera(SCRIPTED_CAMERA);

		if (m_EngineControls->GetKeyState(BENNETT_KEY_F4))
		{
			LevelManager::UnloadLevel(m_World);
			LevelManager::LoadLevel("testLevel.xml", m_World);
		}
	
		if (m_EngineControls->GetKeyState(BENNETT_KEY_F9))
		{
			ServiceLocator::GetRenderer().RecreateSwapChain();
		}	
	}

	World& Engine::GetWorld()
	{
		return m_World;
	}

	LRESULT CALLBACK Engine::WindowsCallbackProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Engine* engine = Engine::GetEngineInstance();

		if (engine == nullptr)
			return DefWindowProc(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_SETFOCUS:
			engine->SetInFocus(true);
			break;

		case WM_KILLFOCUS:
			engine->SetInFocus(false);
			break;

		//case WM_SIZE:
		//{
		//	WORD width = LOWORD(lParam);
		//	WORD height = HIWORD(lParam);
		//	break;
		//}

		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
		case WM_MOUSEWHEEL:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			InputMonitor::Win32InputCallback(message, lParam, wParam);
		}
			break;

		case WM_CLOSE:
			engine->SetIsRunning(false);
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
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	Engine* Engine::CreateEngine(Window& renderWindow)
	{
		if (Engine::m_Instance != nullptr)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}

		m_Instance = new Engine();

		if (!m_Instance->Initialise(renderWindow))
		{
			delete m_Instance;
			m_Instance = nullptr;
		}

		return m_Instance;
	}

	Engine* Engine::GetEngineInstance()
	{
		return m_Instance;
	};
}