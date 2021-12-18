#pragma once
#include "Melone/Core/Event.h"

#include <utility>

namespace Melone
{
	class WindowResizeEvent : public Event
	{
	private:
		std::pair<int32_t, int32_t> mWinDimensions;
	public:
		WindowResizeEvent(int32_t newWidth, int32_t newHeight)
			:
			mWinDimensions(newWidth, newHeight)
		{}
		
		~WindowResizeEvent(void) = default;
	public:
		const std::pair<int32_t, int32_t> getWinDimensions(void) const { return mWinDimensions; }

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