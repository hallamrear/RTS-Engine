#pragma once
#include "Window.h"
#include "ApplicationEvents.h"

#include <chrono>

namespace Bennett
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; };
		inline static Application& Get() { return *s_Instance; };

	private:
		//TODO: WindowCloseEvent for application (declaration)
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Bennett::Window> m_Window;
		bool m_IsRunning = true;

		//Static pointer to this application.
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

