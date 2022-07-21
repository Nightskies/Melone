#pragma once
#include "EventCategory.h"

#include <sstream>

namespace Melone
{
	class WindowResizeEvent
	{
	private:
		std::pair<float, float> mWinDimensions;
	public:
		WindowResizeEvent() = default;

		WindowResizeEvent(int newWidth, int newHeight)
			:
			mWinDimensions(newWidth, newHeight)
		{}
		
		~WindowResizeEvent() = default;

		std::pair<float, float> GetWinDimensions() const { return mWinDimensions; }

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "WindowResizeEvent[" << mWinDimensions.first << "x" << mWinDimensions.second << "]";
			return ss.str();
		}

		static constexpr unsigned char GetCategory() { return EventCategoryApplication; }
	};

	class WindowCloseEvent
	{
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		static constexpr unsigned char GetCategory() { return EventCategoryApplication; }
	};
}