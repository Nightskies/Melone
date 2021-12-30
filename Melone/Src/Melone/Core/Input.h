#pragma once

#include "Core.h"

namespace Melone
{
	class Input
	{
	public:
		static bool isKeyPressed(int keyCode);
		static bool isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition(void);
		static float getMouseX(void);
		static float getMouseY(void);
	};
}
