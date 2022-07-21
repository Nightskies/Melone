#include "mlpch.h"
#include "OpenGLContext.h"

#include "Melone/Core/log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Melone
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:
		mWindow(window)
	{
		glfwMakeContextCurrent(mWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MELONE_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers() const
	{
		glfwSwapBuffers(mWindow);
	}
}