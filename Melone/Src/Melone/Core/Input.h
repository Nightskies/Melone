#pragma once

#include "Melone/Core/Core.h"

namespace Melone
{
	class Input
	{
	public:
		static bool isKeyPressed(int keyCode) { return sInstance->isKeyPressedImpl(keyCode); }
		static bool isMouseButtonPressed(int button) { return sInstance->isMouseButtonPressedImpl(button); }
		static std::pair<float, float> getMousePosition() { return sInstance->getMousePositionImpl(); }
		static float getMouseX() { return sInstance->getMouseXImpl(); }
		static float getMouseY() { return sInstance->getMouseYImpl(); }

	protected:
		virtual bool isKeyPressedImpl(int keyCode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
	private:
		static Input* sInstance;
	};
}
