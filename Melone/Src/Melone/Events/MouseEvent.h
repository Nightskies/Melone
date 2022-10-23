#pragma once
#include "EventCategory.h"

#include <sstream>

namespace Melone
{
	class MouseMovedEvent
	{
	public:
		MouseMovedEvent() = default;

		MouseMovedEvent(double x, double y)
			:
			mX(x),
			mY(y)
		{}

		~MouseMovedEvent() = default;

		std::pair<double, double> GetMousePosition() const { return std::make_pair(mX, mY); }

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseMovedEvent[" << mX << "," << mY << "]";
			return ss.str();
		}

		static constexpr unsigned char GetCategory() { return EventCategoryMouse | EventCategoryInput; }
	private:
		double mX;
		double mY;
	};

	class MouseScrolledEvent
	{
	public:
		MouseScrolledEvent() = default;

		MouseScrolledEvent(double xOffset, double yOffset)
			:
			mXOffset(xOffset),
			mYOffset(yOffset)
		{}

		~MouseScrolledEvent() = default;

		float GetXOffset() const { return mXOffset; }
		float GetYOffset() const { return mYOffset; }

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent[" << mXOffset << "," << mYOffset << "]";
			return ss.str();
		}
		static constexpr unsigned char GetCategory() { return EventCategoryMouse | EventCategoryInput; }
	private:
		double mXOffset;
		double mYOffset;
	};

	class MouseButtonEvent
	{
	public:
		static constexpr unsigned char GetCategory() { return EventCategoryMouse | EventCategoryInput; }

		int GetPressedButton() const { return mButton; }
	protected:
		MouseButtonEvent() = default;

		MouseButtonEvent(int button)
			:
			mButton(button)
		{}

		virtual ~MouseButtonEvent() = default;
	protected:
		int mButton;
	};


	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent() = default;

		MouseButtonPressedEvent(int button)
			:
			MouseButtonEvent(button)
		{}

		~MouseButtonPressedEvent() override = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent[" << mButton << "]";
			return ss.str();
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent() = default;

		MouseButtonReleasedEvent(int button)
			:
			MouseButtonEvent(button)
		{}

		~MouseButtonReleasedEvent() override = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent[" << mButton << "]";
			return ss.str();
		}
	};
}

