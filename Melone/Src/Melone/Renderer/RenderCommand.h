#pragma once
#include "RendererAPI.h"

namespace Melone
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			mRendererAPI->Init();
		}

		static void Clear()
		{
			mRendererAPI->Clear();
		}

		static void SetClearColor(const glm::vec4& color)
		{
			mRendererAPI->SetClearColor(color);
		}

		static void DrawIndexed(const SPtr<VAO>& VAO, unsigned int count = 0)
		{
			mRendererAPI->DrawIndexed(VAO, count);
		}

		static void DrawLines(const SPtr<VAO>& VAO, unsigned int count)
		{
			mRendererAPI->DrawLines(VAO, count);
		}

		static void SetLineWidth(float width)
		{
			mRendererAPI->SetLineWidth(width);
		}

		static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			mRendererAPI->SetViewport(x, y, width, height);
		}
	private:
		RenderCommand() = default;
		~RenderCommand() = default;
	private:
		static RendererAPI* mRendererAPI;
	};
}