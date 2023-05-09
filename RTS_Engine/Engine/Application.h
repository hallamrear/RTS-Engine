#pragma once
#include "Renderer.h"
#include "Entity.h"
#include "Camera.h"

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
		std::vector<Entity> m_Entities;
		Entity Floor;

		bool m_IsRunning;
		GLFWwindow* m_Window;
		Renderer m_Renderer;
		Camera m_Camera;

		bool InitialiseWindow(const WindowDetails& details);
		bool InitialiseRenderer();

		void DestroyWindow();
		void DestroyRenderer();

		void ProcessInput(const float& deltaTime);
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

