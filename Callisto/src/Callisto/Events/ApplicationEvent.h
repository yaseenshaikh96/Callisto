#pragma once

#include "Event.h"

namespace Callisto
{
	class CALLISTO_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width{width}, m_Height{height}
		{}
		inline unsigned int GetWidth() const
		{
			return m_Width;
		}
		inline unsigned int GetHeight() const
		{
			return m_Height;
		}
		std::string ToString() const
		{
			std::stringstream ss{};
			ss << "WindowResizeEvent: " << GetWidth() << ", " << GetHeight();
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class CALLISTO_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(float offsetX, float offsetY)
			: m_OffsetX{ offsetX }, m_OffsetY{ offsetY }
		{}
		inline float GetOffsetX() const
		{
			return m_OffsetX;
		}
		inline float GetOffsetY() const
		{
			return m_OffsetY;
		}
		std::string ToString() const
		{
			std::stringstream ss{};
			ss << "WindowMovedEvent: " << GetOffsetX() << ", " << GetOffsetY();
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class CALLISTO_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	
	class CALLISTO_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class CALLISTO_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class CALLISTO_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(){}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}