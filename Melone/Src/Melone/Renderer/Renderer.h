#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
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
		Renderer(void) = default;
		~Renderer(void) = default;

		static void init(void);
		static void shutdown(void);

		static void onWindowResize(const std::pair<unsigned int, unsigned int>& dimensions);

		static void beginScene(OrthographicCamera& camera);
		static void endScene(void);

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VAO>& VAO, const glm::mat4& transform = glm::mat4(1.0f));

		static const RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};
}
