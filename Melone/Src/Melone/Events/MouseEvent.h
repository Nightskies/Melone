#pragma once
#include "Event.h"

namespace Melone
{
	class MouseMovedEvent : public Event
	{
	private:
		double mX;
		double mY;
	public:
		MouseMovedEvent(double x, double y)
			:
			mX(x),
			mY(y)
		{}

		~MouseMovedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "MouseMovedEvent[" << mX << "," << mY << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class MouseScrolledEvent : public Event
	{
	private:
		double mXOffset;
		double mYOffset;
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			:
			mXOffset(xOffset),
			mYOffset(yOffset)
		{}

		~MouseScrolledEvent(void) = default;

		float getXOffset(void) const { return mXOffset; }
		float getYOffset(void) const { return mYOffset; }

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent[" << mXOffset << "," << mYOffset << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class MouseButtonEvent : public Event
	{
	protected:
		int mButton;
	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			:
			mButton(button)
		{}
		
		~MouseButtonEvent(void) = default;
	};


	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:
			MouseButtonEvent(button)
		{}

		~MouseButtonPressedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent[" << mButton << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:
			MouseButtonEvent(button)
		{}

		~MouseButtonReleasedEvent(void) = default;

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent[" << mButton << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

