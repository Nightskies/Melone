#include "mlpch.h"
#include "Input.h"

#include "Melone/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Melone
{
	bool Input::IsKeyPressed(int keyCode)
	{
		const Window* window = Window::GetInstance();
		return glfwGetKey(window->GetHandle(), keyCode) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		const Window* window = Window::GetInstance();
		return glfwGetMouseButton(window->GetHandle(), button) == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		std::pair<double, double> positions;

		const Window* window = Window::GetInstance();
		glfwGetCursorPos(window->GetHandle(), &positions.first, &positions.second);

		return positions;
	}
}
