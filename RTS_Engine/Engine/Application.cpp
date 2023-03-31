#include "BennettPCH.h"
#include "Application.h"

#include "Logger.h"

namespace Bennett
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		if (s_Instance)
			Log("Application instance already existing!", LOG_CRITICAL);

		m_Window = std::unique_ptr<Window>(Window::Create());

		//m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	void Application::Run()
	{
		float lTime = GetTickCount();
		float cTime = GetTickCount();
		float dTime = GetTickCount();

		while (m_IsRunning)
		{
			cTime = GetTickCount();
			dTime = cTime - lTime;

			Log(dTime, LOG_SPECIAL);

			if (m_Window)
				m_Window->OnUpdate(dTime);
			else
			{
				Log("Window not initialised.", LOG_SERIOUS);
				m_IsRunning = false;
			}

			lTime = cTime;
		}
	}

	void Application::OnEvent(Event& e)
	{
		
		//TODO: Event Dispatcher for windows events.
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	//TODO: Move to be defined in CLIENT using a child application.
	Application * CreateApplication()
	{
		return new Application;
	}
}