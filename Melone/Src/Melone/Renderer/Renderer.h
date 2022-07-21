#pragma once

#include "RenderCommand.h"
#include "Camera/Camera.h"
#include "Shader.h"

namespace Melone
{
	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* mSceneData;
	public:
		Renderer() = default;
		~Renderer() = default;

		static void Init();
		static void Shutdown();

		static void OnWindowResize(const std::pair<unsigned int, unsigned int>& dimensions);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const SPtr<Shader>& shader, const SPtr<VAO>& VAO, const glm::mat4& transform = glm::mat4(1.0f));

		static const RendererAPI::API getAPI() { return RendererAPI::GetAPI(); }
	};
}
