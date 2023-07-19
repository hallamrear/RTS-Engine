#pragma once
#include "Entity.h"
#include "World.h"
#include "CameraController.h"
#include "Window.h"

namespace Bennett
{
	class BENNETT_ENGINE Engine
	{
	private:
		static Engine* m_Instance;

		World m_World;
		CameraController& m_CameraController;
		InputMonitor* m_EngineControls;
		bool m_IsRunning;
		inline static bool m_InFocus = false;

		static Engine* GetEngineInstance();

	public:
		Engine();
		~Engine();

		void ProcessInput(const float& DeltaTime);
		void Update(float DeltaTime);
		void Render();
		inline bool IsRunning() const { return m_IsRunning; };
		inline void SetIsRunning(bool state) { m_IsRunning = state; };
		inline static void SetInFocus(bool state) { m_InFocus = state; };
		inline static bool GetInFocus() { return m_InFocus; };

		bool Initialise(Window& renderWindow);
		void Destroy();

		static LRESULT CALLBACK WindowsCallbackProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		World& GetWorld();

		static Engine* CreateEngine(Window& renderWindow);
	};
}
