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
		~OpenGLContext() = default;

		void SetContext(GLFWwindow* window);

		void SwapBuffers() const;
	};
}