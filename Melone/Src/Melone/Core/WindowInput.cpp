#include "mlpch.h"
#include "Melone/Core/Input.h"


#include "Melone/Core/App.h"
#include <GLFW/glfw3.h>

namespace Melone
{
	bool Input::isKeyPressed(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition(void)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float Input::getMouseX(void)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return (float)xPos;
	}

	float Input::getMouseY(void)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return (float)yPos;
	}
}
