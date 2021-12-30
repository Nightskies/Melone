#include "mlpch.h"
#include "Framebuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Melone
{
	std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::Undefined:
			MELONE_CORE_ASSERT(false, "RendererAPI is undefined");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLFramebuffer>(spec);

		default:
			MELONE_CORE_ASSERT(false, "Unknown renderer api");
			return nullptr;
		}
	}
}
