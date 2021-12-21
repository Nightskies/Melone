#pragma once

#include "Melone/Core/Layer.h"

#include "Melone/Events/KeyEvent.h"
#include "Melone/Events/MouseEvent.h"

namespace Melone
{
	class ImGuiLayer: public Layer
	{
	public:
		ImGuiLayer(void)
			:
			Layer("ImGui")
		{}

		~ImGuiLayer(void) = default;

		virtual void onAttach(void) override;
		virtual void onDetach(void) override;
		virtual void onUpdate(void) override;
	};
}
