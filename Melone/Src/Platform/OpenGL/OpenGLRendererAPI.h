#pragma once
#include "Melone/Renderer/RendererAPI.h"

namespace Melone
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		~OpenGLRendererAPI() = default;

		virtual void Init() override;

		virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		void Clear() override;
		void SetClearColor(const glm::vec4& color) override;

		void DrawIndexed(const SPtr<VAO>& VAO, unsigned int indexCount = 0) override;
		void DrawLines(const SPtr<VAO>& VAO, unsigned int vertexCount) override;

		void SetLineWidth(float width) override;
	};
}