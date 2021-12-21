#pragma once
#include "Melone/Core/Event.h"

namespace Melone
{
	class KeyEvent : public Event
	{
	protected:
		int mKeyCode;
	protected:
		KeyEvent(int keyCode)
			:
			mKeyCode(keyCode)
		{}

		~KeyEvent(void) = default;
	public:
		int getKeyCode(void) const { return mKeyCode; }
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		static int repeatCount;
	public:
		KeyPressedEvent(int keyCode, int repeat)
			:
			KeyEvent(keyCode)
		{
			if (repeat)
				repeatCount++;
			else
				repeatCount = 0;
		}

		~KeyPressedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "KeyPressedEvent[" << mKeyCode << " (" << repeatCount << " repeats)]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			:
			KeyEvent(keyCode)
		{}

		~KeyReleasedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent[" << mKeyCode << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			:
			KeyEvent(keyCode)
		{}

		~KeyTypedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "KeyTypedEvent[" << mKeyCode << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}