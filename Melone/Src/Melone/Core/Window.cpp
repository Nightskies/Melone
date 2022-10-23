#include "mlpch.h"
#include "Window.h"

#include "Melone/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Melone
{
	Window::Window(WindowProperties&& winProps, WindowType&& wType)
		:
		mProperties(std::move(winProps)),
		mWindowType(std::move(wType))
	{
		int success = glfwInit();
		MELONE_CORE_ASSERT(success, "Could't initialize GLFW!");

		auto&& GLFWErrorCallback = [](int error, const char* description)
		{
			MELONE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		};

		glfwSetErrorCallback(GLFWErrorCallback);

		auto [width, height] = mProperties.Dimensions;
		auto title = mProperties.Title.c_str();

		mHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
		MELONE_CORE_ASSERT(mHandle, "Failed create window!");

		mContextType.SetContext(mHandle);

		mInstance = this;

		MELONE_CORE_INFO("Created window {0} ({1}, {2})", title, width, height);

		EventSystem::Subscribe(this, &Window::OnResize);

		EventSystem::Subscribe(this, &Window::OnClose);

		glfwSetWindowSizeCallback(mHandle, [](GLFWwindow* window, int width, int height)
		{
			EventSystem::Publish<Event::WindowResize>(width, height);
		});

		glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window)
		{
			EventSystem::Publish<Event::WindowClose>();
		});

		glfwSetKeyCallback(mHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					EventSystem::Publish<Event::KeyPressed>(key, 0);
					break;
				}
				case GLFW_RELEASE:
				{
					EventSystem::Publish<Event::KeyReleased>(key);
					break;
				}
				case GLFW_REPEAT:
				{
					EventSystem::Publish<Event::KeyPressed>(key, 1);
					break;
				}
			}
		});

		glfwSetCharCallback(mHandle, [](GLFWwindow* window, unsigned int keyCode)
		{
			EventSystem::Publish<Event::KeyTyped>(keyCode);
		});

		glfwSetMouseButtonCallback(mHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					EventSystem::Publish<Event::MouseButtonPressed>(button);
					break;
				}
				case GLFW_RELEASE:
				{
					EventSystem::Publish<Event::MouseButtonReleased>(button);
					break;
				}
			}
		});

		glfwSetScrollCallback(mHandle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			EventSystem::Publish<Event::MouseScrolled>(xOffset, yOffset);
		});

		glfwSetCursorPosCallback(mHandle, [](GLFWwindow* window, double xPos, double yPos)
		{
			EventSystem::Publish<Event::MouseMoved>(xPos, yPos);
		});
	}

	void Window::OnResize(const WindowResizeEvent& e)
	{
		mProperties.Dimensions = e.GetWinDimensions();

		auto [width, height] = mProperties.Dimensions;

		if (width == 0 || height == 0)
		{
			mMinimized = true;
		}
		else if (mMinimized)
		{
			mMinimized = false;
		}

		Renderer::OnWindowResize(mProperties.Dimensions);
	}

	void Window::OnClose(const WindowCloseEvent& e)
	{
		mClosed = true;
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(mHandle);
		
		glfwTerminate();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		mVSync = enabled;
	}

	void Window::Update()
	{
		if (!mClosed)
		{
			mContextType.SwapBuffers();

			glfwPollEvents();
		}
	}

	Window::~Window()
	{
		Shutdown();
	}
}