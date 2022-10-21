#pragma once
#include "Timestep.h"
#include "Core.h"

#include <string>

namespace Melone
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		Layer(std::string&& name)
			:
			mName(std::move(name))
		{}

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnAttach() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnDetach() = 0;

		constexpr bool operator==(const Layer& r) const
		{
			return *this == r;
		}

		const std::string& GetName() const { return mName; }
	protected:
		std::string mName;
	};
}
