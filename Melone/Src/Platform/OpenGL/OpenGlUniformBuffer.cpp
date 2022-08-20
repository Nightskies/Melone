#include "mlpch.h"
#include "OpenGlUniformBuffer.h"

#include <glad/glad.h>

namespace Melone 
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(unsigned int size, unsigned int binding)
	{
		glCreateBuffers(1, &mRendererID);
		glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mRendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, unsigned int size, unsigned int offset)
	{
		glNamedBufferSubData(mRendererID, offset, size, data);
	}
}