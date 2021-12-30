#include "mlpch.h"
#include "Window.h"

#include "Log.h"

#include "Melone/Events/KeyEvent.h"
#include "Melone/Events/MouseEvent.h"
#include "Melone/Events/WindowEvent.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Melone
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		MELONE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const std::string& title, int width, int height)
		:
		mTitle(title),
		mWinDimensions(width, height)
	{
		int success = glfwInit();
		MELONE_CORE_ASSERT(success, "Could't initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		mWindow = glfwCreateWindow(mWinDimensions.first, mWinDimensions.second, mTitle.c_str(), nullptr, nullptr);
		MELONE_CORE_ASSERT(mWindow, "Failed create window!");

		MELONE_CORE_INFO("Created window {0} ({1}, {2})", mTitle, mWinDimensions.first, mWinDimensions.second);

		mContext = new OpenGLContext(mWindow);
		mContext->init();

		glfwSetWindowUserPointer(mWindow, this);
		setVSync(true);

		// Set GLFW Callbacks

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));
			data.setWinDimentions({ width, height });

			WindowResizeEvent event(width, height);
			data.mEventCallback(event);
		});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			WindowCloseEvent event;
			data.mEventCallback(event);
		});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.mEventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.mEventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.mEventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keyCode)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			KeyTypedEvent event(keyCode);
			data.mEventCallback(event);
		});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.mEventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.mEventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			MouseScrolledEvent event(xOffset, yOffset);
			data.mEventCallback(event);
		});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window& data = *(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			MouseMovedEvent event(xPos, yPos);
			data.mEventCallback(event);
		});
	}

	void Window::shutdown(void)
	{
		glfwDestroyWindow(mWindow);
	}

	void Window::update(void)
	{
		mContext->swapBuffers();
		glfwPollEvents();
	}

	void Window::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		mVSync = enabled;
	}
}