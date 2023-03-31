#pragma once
#include "Event.h"

namespace Bennett
{
	using EventCallbackFunction = std::function<void(Event&)>;

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

	//Interface
	class Window
	{
	public:

		virtual ~Window() = default;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(EventCallbackFunction callbackFunc) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowDetails& props = WindowDetails())
		{
			return nullptr;
		}

		void OnUpdate(float deltaTime)
		{

		}
	};

}

