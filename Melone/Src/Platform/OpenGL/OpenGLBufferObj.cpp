#include "mlpch.h"
#include "OpenGLBufferObj.h"

#include <glad/glad.h>

namespace Melone
{
	OpenGLVBO::OpenGLVBO(float* vertices, unsigned int size)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVBO::OpenGLVBO(unsigned int size)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGLVBO::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLVBO::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVBO::setData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	OpenGLVBO::~OpenGLVBO(void)
	{
		glDeleteBuffers(1, &mRendererID);
	}

	/* OpenGLIBO */

	OpenGLIBO::OpenGLIBO(unsigned int* indices, unsigned int count)
		:
		mCount(count)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	void OpenGLIBO::bind(void) const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLIBO::unbind(void) const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLIBO::~OpenGLIBO(void)
	{
		glDeleteBuffers(1, &mRendererID);
	}
}