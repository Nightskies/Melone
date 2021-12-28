#pragma once

#include "RendererAPI.h"

namespace Melone
{
	class RenderCommand
	{
	private:
		static RendererAPI* sRendererAPI;
	public:
		static void init(void)
		{
			sRendererAPI->init();
		}

		static void clear(void)
		{
			sRendererAPI->clear();
		}

		static void setClearColor(const glm::vec4& color)
		{
			sRendererAPI->setClearColor(color);
		}

		static void drawIndexed(const std::shared_ptr<VAO>& VAO, unsigned int count = 0)
		{
			sRendererAPI->drawIndexed(VAO, count);
		}

		static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			sRendererAPI->setViewport(x, y, width, height);
		}
	};
}