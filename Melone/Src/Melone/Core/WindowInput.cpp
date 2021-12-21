#include "WindowInput.h"

#include "Melone/Core/App.h"
#include <GLFW/glfw3.h>

namespace Melone
{
	Input* Input::sInstance = new WindowInput();

	bool WindowInput::isKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowInput::isMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowInput::getMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float WindowInput::getMouseXImpl()
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return (float)xPos;
	}

	float WindowInput::getMouseYImpl()
	{
		auto window = static_cast<GLFWwindow*>(App::getInstance().getWindow().getNativeWindow());
		double xPos;
		double yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return (float)yPos;
	}
}
