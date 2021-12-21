#pragma once

#include "Melone/Core/Input.h"

namespace Melone
{

	class WindowInput : public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keyCode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> getMousePositionImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};

}
