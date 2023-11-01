#pragma once
#include <System/Event.h>

namespace Bennett
{
	class BENNETT_ENGINE WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{
			m_EventType = EVENT_TYPE::EVENT_WINDOW_RESIZE;
		};

		inline virtual void Print()
		{
			Log("LOG :: WINDOW RESIZE EVENT", LOG_SAFE);
		}

		inline unsigned int GetWidth() { return m_Width; };
		inline unsigned int GetHeight() { return m_Height; };

	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class BENNETT_ENGINE WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
			m_EventType = EVENT_TYPE::EVENT_WINDOW_CLOSE;
		};

		inline virtual void Print()
		{
			Log("LOG :: WINDOW CLOSE EVENT", LOG_SAFE);
		}
	};
}