#pragma once

struct GLFWwindow;

namespace Melone
{
	class OpenGLContext
	{
	private:
		GLFWwindow* mWindow = nullptr;
	public:
		OpenGLContext() = default;
		OpenGLContext(GLFWwindow* window);
		~OpenGLContext() = default;

		void SwapBuffers() const;
	};
}
