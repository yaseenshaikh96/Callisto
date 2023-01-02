#pragma once

#include "Event.h"

namespace Callisto
{
	class CALLISTO_API KeyEvent : public Event
	{
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode)
		{}
	public:
		inline int GetKeyCode() const
		{
			return m_KeyCode;
		}	
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		int m_KeyCode;
	};

	class CALLISTO_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatKey)
			: KeyEvent(keyCode), m_RepeatKey(repeatKey)
		{}
		inline int GetRepeatCount() const
		{
			return m_RepeatKey;
		}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "( " << m_RepeatKey << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatKey;
	};

	class CALLISTO_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	};

	class CALLISTO_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode)
		{}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}