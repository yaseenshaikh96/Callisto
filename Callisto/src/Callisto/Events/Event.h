#pragma once

#include "CallistoPCH.h"

#include "Callisto/Core.h"

namespace Callisto
{
	// Every single event represented by an enum value
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled 
	};

	// Used for efficient filtering of events
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Events are blocking, processed instantly
	class CALLISTO_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; // should be in debug only
		virtual int GetCategoryFlags() const = 0; // should be in debug only
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			// GetFlag() == 100100, category = 000100 => return = 000100, that is greater than 0 so casted to true
			return GetCategoryFlags() & category; 
		}
	public: // TODO: make this rotected without breaking EventDispatcher
		bool Handled = false;
	};

	class EventDispacther
	{
		// function that returns a bool and takes a t_EventType&
		template <typename t_EventType>
		using EventFn = std::function<bool(t_EventType&)>; 
	public:
		EventDispacther(Event& e)
			: m_Event(e)
		{}
		template <typename t_EventType>
		bool Dispatch(EventFn<t_EventType> func)
		{
			if (m_Event.GetEventType() == t_EventType::GetStaticType())
			{
				m_Event.Handled = func(*(t_EventType*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}