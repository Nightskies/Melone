#include "mlpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Melone
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const SPtr<VAO>& VAO, unsigned int indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}