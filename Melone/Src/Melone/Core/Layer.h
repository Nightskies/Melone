#pragma once
#include "Event.h"

#include <string>

namespace Melone
{
	class Layer
	{
	protected:
		std::string mName;
	public:
		Layer(std::string name)
			:
			mName(std::move(name))
		{}

		virtual ~Layer(void) = default;

		virtual void onUpdate(void) {};
		virtual void onEvent(Event& e) {};
		virtual void onAttach(void) {};
		virtual void onDetach(void) {};

		const std::string getName(void) const { return mName; }
	};
}
