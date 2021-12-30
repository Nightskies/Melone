#pragma once
#include "Event.h"

#include <utility>

namespace Melone
{
	class WindowResizeEvent : public Event
	{
	private:
		std::pair<int, int> mWinDimensions;
	public:
		WindowResizeEvent(int newWidth, int newHeight)
			:
			mWinDimensions(newWidth, newHeight)
		{}
		
		~WindowResizeEvent(void) = default;

		const std::pair<int, int> getWinDimensions(void) const { return mWinDimensions; }

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "WindowResizeEvent[" << mWinDimensions.first << "x" << mWinDimensions.second << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(void) = default;
		~WindowCloseEvent(void) = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}