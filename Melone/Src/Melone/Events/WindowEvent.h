#pragma once
#include "Melone/Core/Event.h"

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
	public:
		const std::pair<int, int> getWinDimensions(void) const { return mWinDimensions; }

		std::string toString(void) const
		{
			std::stringstream ss;
			ss << "WindowResizeEvent[" << mWinDimensions.first << "x" << mWinDimensions.second << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(void) = default;
		~WindowCloseEvent(void) = default;

		EVENT_CLASS_TYPE(WindowClose)
	};
}