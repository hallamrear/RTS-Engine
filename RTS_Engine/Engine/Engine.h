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
		World m_World;
		CameraController& m_CameraController;
		InputMonitor* m_EngineControls;
		bool m_IsRunning;

	public:
		Engine();
		~Engine();

		void ProcessInput(const float& DeltaTime);
		void Update(float DeltaTime);
		void Render();
		inline bool IsRunning() const { return m_IsRunning; };

		bool Initialise(Window& renderWindow);
		void Destroy();

		World& GetWorld();
	};

	inline Engine* CreateEngine(Window& renderWindow)
	{
		Engine* app = new Engine();

		if (!app->Initialise(renderWindow))
		{
			delete app;
			app = nullptr;
		}

		return app;
	}
}
