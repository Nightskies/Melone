#include "mlpch.h"
#include "Window.h"

#include "Melone/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Melone
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		MELONE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(WindowProperties&& winProps)
		:
		mProperties(std::move(winProps)),
		mWindowType(Windows())
	{
		int success = glfwInit();
		MELONE_CORE_ASSERT(success, "Could't initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		auto [width, height] = mProperties.mDimensions;
		auto title = mProperties.mTitle.c_str();

		mNativeWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
		MELONE_CORE_ASSERT(mNativeWindow, "Failed create window!");

		mContextType = OpenGLContext(mNativeWindow);

		MELONE_CORE_INFO("Created window {0} ({1}, {2})", title, width, height);

		EventSystem::Subscribe(this, &Window::OnResize);

		EventSystem::Subscribe(this, &Window::OnClose);

		glfwSetWindowSizeCallback(mNativeWindow, [](GLFWwindow* window, int width, int height)
		{
			EventSystem::Publish<Event::WindowResize>(width, height);
		});

		glfwSetWindowCloseCallback(mNativeWindow, [](GLFWwindow* window)
		{
			EventSystem::Publish<Event::WindowClose>();
		});

		glfwSetKeyCallback(mNativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

		glfwSetCharCallback(mNativeWindow, [](GLFWwindow* window, unsigned int keyCode)
		{
			EventSystem::Publish<Event::KeyTyped>(keyCode);
		});

		glfwSetMouseButtonCallback(mNativeWindow, [](GLFWwindow* window, int button, int action, int mods)
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

		glfwSetScrollCallback(mNativeWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			EventSystem::Publish<Event::MouseScrolled>(xOffset, yOffset);
		});

		glfwSetCursorPosCallback(mNativeWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			EventSystem::Publish<Event::MouseMoved>(xPos, yPos);
		});
	}

	const Window& Window::GetInstance(std::optional<WindowProperties>&& winProps)
	{
		static std::optional<Window> instance;

		if (instance)
		{
			MELONE_CORE_ASSERT(!winProps, "Passing data when the instance has already been constructed");

			return instance.value();
		}
		else if (winProps)
		{
			return instance.emplace(std::move(*winProps));
		}
	}

	void Window::OnResize(const WindowResizeEvent& e)
	{
		mProperties.mDimensions = e.GetWinDimensions();

		auto [width, height] = mProperties.mDimensions;

		if (width == 0 || height == 0)
		{
			mMinimized = true;
		}
		else if (mMinimized)
		{
			mMinimized = false;
		}

		Renderer::OnWindowResize(mProperties.mDimensions);
	}

	void Window::OnClose(const WindowCloseEvent& e)
	{
		mClosed = true;

		glfwDestroyWindow(mNativeWindow);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		mVSync = enabled;
	}

	void Window::Update() const
	{
		if (!mClosed)
		{
			std::visit([](const auto& context) { context.SwapBuffers(); }, mContextType);

			glfwPollEvents();
		}
	}
}