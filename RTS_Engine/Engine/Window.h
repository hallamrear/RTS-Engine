#pragma once
#include "WindowDetails.h"
#include <Windows.h>
#include "Logger.h"
#include <glm/glm.hpp>

namespace Bennett
{
	class BENNETT_ENGINE Window
	{
	private:
		HWND m_WindowHandle;
		bool m_WaitingToClose;

		Window();

		bool Initialise(const WindowDetails& details);
		void Destroy();

        inline static HWND CreateWin32WindowHandle(HINSTANCE hInstance, const Bennett::WindowDetails& details)
		{
			HWND parentHandle = NULL;

			if (details.Parent != nullptr)
			{
				parentHandle = details.Parent->GetWindowHandle();
			}

			HWND windowHandle = CreateWindow(
				details.ClassDetails.ClassName,
				details.Title,
				details.WindowStyles,
				details.X, details.Y,
				details.Width, details.Height,
				parentHandle,
				nullptr,
				hInstance,
				details.lpParam);

			if (!windowHandle)
			{
				Log(GetLastWin32Error(), LOG_SERIOUS);
				return FALSE;
			}

			ShowWindow(windowHandle, details.ShowState);
			UpdateWindow(windowHandle);

			return windowHandle;
		}


		inline static ATOM RegisterWin32Class(HINSTANCE hInstance, const Bennett::WindowDetails& windowDetails)
		{
			WNDCLASSEX wcex{};

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | windowDetails.ClassDetails.AdditionalClassStyles;
			wcex.lpfnWndProc = windowDetails.ClassDetails.WndProcCallback;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(windowDetails.ClassDetails.Icon));
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = windowDetails.ClassDetails.BackgroundColour;
			wcex.lpszMenuName = MAKEINTRESOURCE(windowDetails.ClassDetails.MenuName);
			wcex.lpszClassName = windowDetails.ClassDetails.ClassName;
			wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(windowDetails.ClassDetails.SmallIcon));

			ATOM registeredClass = RegisterClassEx(&wcex);

			if (!registeredClass)
			{
				Log(GetLastWin32Error(), LOG_SERIOUS);
				return 0;
			}

			return registeredClass;
		};

	public:
		~Window();		

		void SetTitle(const char* title);
		void SetSize(const glm::vec2& size);
		void SetPosition(const glm::vec2& position);
		glm::vec2 GetSize() const;
		glm::vec2 GetPosition() const;
		const bool ShouldClose() const;
		void Close();
		const HWND& GetWindowHandle() const;

		inline static Window* CreateWin32Window(const WindowDetails& details)
		{
			Bennett::Window* window = new Bennett::Window();

			if (!window->Initialise(details))
			{
				delete window;
				window = nullptr;
			}

			return window;
		};
	};
}

//Bennett::Window* Bennett::Window::CreateWin32Window(const Bennett::WindowDetails& details)
//HWND Bennett::Window::CreateWin32WindowHandle(HINSTANCE hInstance, const Bennett::WindowDetails& details)
//ATOM Bennett::Window::RegisterWin32Class(HINSTANCE hInstance, const Bennett::WindowDetails& windowDetails)