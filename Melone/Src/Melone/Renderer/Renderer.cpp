#include "mlpch.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(const std::pair<unsigned int, unsigned int>& dimensions)
	{
		auto [width, height] = dimensions;

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		mSceneData->ViewProjectionMatrix = camera.GetProjection();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const SPtr<Shader>& shader, const SPtr<VAO>& VAO, const glm::mat4& transform)
	{
		shader->Bind();
		std::static_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("uViewProjection", mSceneData->ViewProjectionMatrix);
		std::static_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("uTransform", transform);
		VAO->Bind();

		RenderCommand::DrawIndexed(VAO);
	}

	void Renderer::Shutdown()
	{}
}