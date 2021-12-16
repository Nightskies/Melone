#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Melone
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:
		mWindow(window)
	{}

	void OpenGLContext::init(void)
	{
		glfwMakeContextCurrent(mWindow);
	}

	void OpenGLContext::swapBuffers(void)
	{
		glfwSwapBuffers(mWindow);
	}
}