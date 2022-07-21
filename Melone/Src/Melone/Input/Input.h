#pragma once

#include <utility>

namespace Melone
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
	};
}
