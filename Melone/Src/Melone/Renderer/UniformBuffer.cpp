#include "mlpch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Melone
{
	SPtr<UniformBuffer> UniformBuffer::Create(unsigned int size, unsigned int binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::Undefined:
			MELONE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:  
			return std::make_shared<OpenGLUniformBuffer>(size, binding);
		}

		MELONE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}