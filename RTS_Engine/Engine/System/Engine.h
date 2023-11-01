#pragma once
#include <World/Entity.h>
#include <World/World.h>
#include <Camera/CameraController.h>
#include <Rendering/Window.h>

namespace Bennett
{
	class BENNETT_ENGINE Engine
	{
	protected:
		static Engine* m_Instance;

		World m_World;
		CameraController& m_CameraController;
		InputMonitor* m_EngineControls;
		bool m_IsRunning;
		inline static bool m_InFocus;

		Engine();
		bool InitialiseEngineSystems(Window& renderWindow);
		void Destroy();

	public:
		virtual ~Engine() = 0;
		virtual bool Initialise() = 0;
		static Engine* GetEngineInstance();

		void ProcessInput(const float& DeltaTime);
		void Update(float DeltaTime);
		void Render();

		inline bool IsRunning() const { return m_IsRunning; };
		inline void SetIsRunning(bool state) { m_IsRunning = state; };
		inline static void SetInFocus(bool state) 
		{
			m_InFocus = state;

			if (ENABLE_LOG_FOCUS)
			{
				Log("Focus: " + std::to_string(m_InFocus), LOG_SAFE);
			}
		};
		inline static bool GetInFocus() { return m_InFocus; };

		static LRESULT CALLBACK WindowsCallbackProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		World& GetWorld();
	};
}
