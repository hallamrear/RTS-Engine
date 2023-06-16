#pragma once
#include "WindowDetails.h"

namespace Bennett
{

	class Window
	{
	private:
		GLFWwindow* m_Window;

		void Initialise();
		void Destroy();

	public:
		Window();
		Window(const WindowDetails& details);
		~Window();		

		inline GLFWwindow& GetGLFWWindow() { return *m_Window; };
		void SetWindowDetails(const WindowDetails& details);
		void SetTitle(const std::string& title);
		void SetTitle(const char* title);
		void SetSize(const glm::vec2 size);
		const bool ShouldClose() const;
		void Close() const;
	};
}