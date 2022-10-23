#pragma once
#include "Melone/Renderer/UniformBuffer.h"

namespace Melone 
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(unsigned int size, unsigned int binding);
		~OpenGLUniformBuffer();

		void SetData(const void* data, unsigned int size, unsigned int offset = 0) override;
	private:
		unsigned int mRendererID = 0;
	};
}