#include "mlpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::init(void)
	{
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResize(const std::pair<unsigned int, unsigned int>& dimensions)
	{
		RenderCommand::setViewport(0, 0, dimensions.first, dimensions.second);
	}

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		mSceneData->ViewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene(void)
	{

	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VAO>& VAO, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("uTransform", transform);
		VAO->bind();

		RenderCommand::drawIndexed(VAO);
	}

	void Renderer::shutdown(void)
	{

	}
}