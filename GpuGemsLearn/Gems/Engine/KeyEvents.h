#pragma once
#include "Events.h"
#include "KeyCodes.h"
#include <sstream>

class KeyEvent : public Event {
public:

	KeyCode GetKeyCode() const { return m_iKeyCode; }
	inline unsigned int GetMode() const { return m_iMode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(KeyCode keycode, int mode)
		:m_iKeyCode(keycode), m_iMode(mode) {}
	int m_iKeyCode;
	int m_iMode;
};

class KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent(int keycode, int mode, int repeatCount)
		: KeyEvent(keycode, mode), m_iRepeatCount(repeatCount) {}

	inline unsigned int GetRepeatCount() const { return m_iRepeatCount; }
	std::string ToString() const override {
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_iKeyCode << "(" << m_iRepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
protected:
	int m_iRepeatCount;
};


class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const int keycode, int mode)
		: KeyEvent(keycode, mode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_iKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const int keycode, int mode)
		: KeyEvent(keycode, mode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_iKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};