#pragma once
#include "BennettPCH.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Bennett
{
	enum class EVENT_TYPE
	{
		EVENT_NONE = 0,
		EVENT_WINDOW_CLOSE, EVENT_WINDOW_RESIZE,
		EVENT_KEY_PRESSED, EVENT_KEY_RELEASED,
		EVENT_MOUSE_PRESSED, EVENT_MOUSE_BUTTON_RELEASED, EVENT_MOUSE_MOVED, EVENT_MOUSE_SCROLLED
	};

	class BENNETT_ENGINE Event
	{

	protected:
		virtual ~Event() = default;
		virtual EVENT_TYPE GetEventType() const { return m_EventType; };

		virtual void Print() = 0;

		EVENT_TYPE m_EventType = EVENT_TYPE::EVENT_NONE;
		bool m_Handled = 0;
	};


	class BENNETT_ENGINE EventDispatcher
	{
		//Using EventFunction as the callback for the event.
		template<typename T>
		using EventFunction = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
			//Creating Dispatcher for event.
		}

		//templated to fit any event provided it matches the event function parameters
		template<typename T>
		bool Dispatch(EventFunction<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}
