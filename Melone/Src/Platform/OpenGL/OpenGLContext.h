#pragma once

struct GLFWwindow;

namespace Melone
{
	class OpenGLContext
	{
	public:
		OpenGLContext() = default;
		~OpenGLContext() = default;

		void SetContext(GLFWwindow* window);

		void SwapBuffers() const;
	private:
		GLFWwindow* mWindow = nullptr;
	};
}