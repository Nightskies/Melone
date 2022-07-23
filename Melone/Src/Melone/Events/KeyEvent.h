#pragma once
#include "EventCategory.h"

#include <sstream>

namespace Melone
{
	class KeyEvent
	{
	protected:
		int mKeyCode;
	protected:
		KeyEvent() = default;

		KeyEvent(int keyCode)
			:
			mKeyCode(keyCode)
		{}

		virtual ~KeyEvent() = default;
	public:
		int GetKeyCode() const { return mKeyCode; }

		static constexpr unsigned char GetCategory() { return EventCategoryKeyboard | EventCategoryInput; }
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		static inline int RepeatCount;
	public:
		KeyPressedEvent() = default;

		KeyPressedEvent(int keyCode, int repeat)
			:
			KeyEvent(keyCode)
		{
			if (repeat)
				RepeatCount++;
			else
				RepeatCount = 0;
		}

		bool IsKeyRepeat() const { return RepeatCount ? true : false; }

		~KeyPressedEvent() override = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyPressedEvent[" << mKeyCode << " (" << RepeatCount << " repeats)]";
			return ss.str();
		}
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent() = default;

		KeyReleasedEvent(int keyCode)
			:
			KeyEvent(keyCode)
		{}

		~KeyReleasedEvent() override = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent[" << mKeyCode << "]";
			return ss.str();
		}
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent() = default;

		KeyTypedEvent(int keyCode)
			:
			KeyEvent(keyCode)
		{}

		~KeyTypedEvent() override = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyTypedEvent[" << mKeyCode << "]";
			return ss.str();
		}
	};
}