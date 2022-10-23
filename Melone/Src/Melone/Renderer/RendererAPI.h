#pragma once
#include "ArrayObj.h"

#include <glm/glm.hpp>

namespace Melone
{
	class RendererAPI
	{
	public:
		enum class API
		{
			Undefined = 0,
			OpenGL
		};
	public:
		RendererAPI() = default;
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void DrawIndexed(const SPtr<VAO>& VAO, unsigned int indexCount = 0) = 0;

		virtual void DrawLines(const SPtr<VAO>& VAO, unsigned int vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return mAPI; }
	private:
		static inline API mAPI = API::OpenGL;
	};
}
