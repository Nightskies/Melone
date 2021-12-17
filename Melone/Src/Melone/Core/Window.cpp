#include "Window.h"

#include "Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Melone
{
	Window::Window(int32_t width, int32_t height, std::string title)
		:
		mTitle(std::move(title)),
		mHeight(height),
		mWidth(width)
	{
		int success = glfwInit();
		MELONE_CORE_ASSERT(success, "Could't initialize GLFW!");

		mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
		MELONE_CORE_ASSERT(mWindow, "Failed create window!");

		MELONE_CORE_INFO("Created window {0} ({1}, {2})", mTitle, mWidth, mHeight);

		mContext = new OpenGLContext(mWindow);
		mContext->init();

		setViewport(mWidth, mHeight);
		setClearColor();
	}

	void Window::shutdown(void)
	{
		glfwDestroyWindow(mWindow);
	}

	void Window::update(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		mContext->swapBuffers();
		glfwPollEvents();
	}

	void Window::setViewport(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}