#include "mlpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	void Renderer::init(void)
	{
		RenderCommand::init();
	}

	void Renderer::onWindowResize(const std::pair<unsigned int, unsigned int>& dimensions)
	{
		RenderCommand::setViewport(0, 0, dimensions.first, dimensions.second);
	}

	void Renderer::beginScene(void)
	{

	}

	void Renderer::endScene(void)
	{

	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VAO>& VAO)
	{
		shader->bind();
		VAO->bind();

		RenderCommand::drawIndexed(VAO);
	}

	void Renderer::shutdown(void)
	{

	}
}