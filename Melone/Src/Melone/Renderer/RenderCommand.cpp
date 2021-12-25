#include "mlpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Melone
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}