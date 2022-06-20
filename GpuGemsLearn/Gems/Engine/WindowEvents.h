#pragma once
#include "Events.h"
#include <sstream>

class WindowResizeEvent : public Event {
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_iWidth(width), m_iHeight(height) {}
	inline unsigned int GetWidth() const { return m_iWidth; }
	inline unsigned int GetHeight() const { return m_iHeight; }

	std::string ToString() const override {
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_iWidth << ", " << m_iHeight;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	unsigned int m_iWidth, m_iHeight;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};