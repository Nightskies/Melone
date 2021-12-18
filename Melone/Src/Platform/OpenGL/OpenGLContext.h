#pragma once
#include "Melone/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Melone
{
	class OpenGLContext : public IGraphicsContext
	{
	public:
		GLFWwindow* mWindow;
	public:
		OpenGLContext(GLFWwindow* window);
		~OpenGLContext(void) = default;

		virtual void init(void) override;
		virtual void swapBuffers(void) override;
	};
}
