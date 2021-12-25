#pragma once

#include "RenderCommand.h"
#include "Shader.h"

namespace Melone
{
	class Renderer
	{
	public:
		Renderer(void) = default;
		~Renderer(void) = default;

		static void init(void);
		static void shutdown(void);

		static void onWindowResize(const std::pair<unsigned int, unsigned int>& dimensions);

		static void beginScene(void);
		static void endScene(void);

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VAO>& VAO);

		static const RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};
}
