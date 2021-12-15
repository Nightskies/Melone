#include "App.h"

#include <GLFW/glfw3.h>

namespace Melone
{
	void App::run(void)
	{
		int success = glfwInit();

		if (success)
		{
			GLFWwindow* mWindow = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
			glfwMakeContextCurrent(mWindow);

			glViewport(0, 0, 800, 600);

			glClearColor(1.0, 0.0, 1.0, 1.0);

			while (!glfwWindowShouldClose(mWindow))
			{
				glClear(GL_COLOR_BUFFER_BIT);
				glfwSwapBuffers(mWindow);

				glfwPollEvents();
			}

			glfwTerminate();
		}

		while (true);
	}
}