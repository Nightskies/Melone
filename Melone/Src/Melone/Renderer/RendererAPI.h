#pragma once

#include <glm/glm.hpp>
#include "ArrayObj.h"

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
	private:
		static API sAPI;
	public:
		RendererAPI(void) = default;
		virtual ~RendererAPI(void) = default;
	public:
		virtual void init(void) = 0;
		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
		virtual void clear(void) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;

		virtual void drawIndexed(const std::shared_ptr<VAO>& VAO) = 0;

		static API getAPI(void) { return sAPI; }
	};
}
