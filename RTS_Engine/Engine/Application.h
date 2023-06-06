#pragma once
#include "Renderer.h"
#include "Entity.h"
#include "World.h"
#include "CameraController.h"

struct GLFWwindow;

namespace Bennett
{
	struct WindowDetails
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowDetails(
			const std::string& title = "Bennett Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	class Application
	{
	private:
		World m_World;

		bool m_IsRunning;
		GLFWwindow* m_Window;
		Renderer m_Renderer;
		CameraController& m_CameraController;
		InputMonitor* m_ApplicationControls;

		bool InitialiseWindow(const WindowDetails& details);
		bool InitialiseRenderer();

		void DestroyWindow();
		void DestroyRenderer();

		void ProcessInput(const float& DeltaTime);
		void Update(float DeltaTime);
		void Render();

	public:
		Application();
		~Application();

		void GameLoop();

		bool Initialise(int argc, char** argv, const WindowDetails& details);
		void Destroy();
	};

	Application* CreateApplication(int argc, char** argv, const WindowDetails& details);
}