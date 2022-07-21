#include "mlpch.h"
#include "ArrayObj.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLArrayObj.h"

namespace Melone
{
	SPtr<VAO> VAO::Create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::Undefined:
				MELONE_CORE_ASSERT(false, "RendererAPI is undefined");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVAO>();

			default:
				MELONE_CORE_ASSERT(false, "Unknown renderer api");
				return nullptr;
		}
	}
}