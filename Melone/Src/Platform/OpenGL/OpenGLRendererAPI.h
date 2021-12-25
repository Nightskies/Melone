#pragma once

#include "Melone/Renderer/RendererAPI.h"

namespace Melone
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init(void) override;

		virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		void clear(void) override;
		void setClearColor(const glm::vec4& color) override;

		void drawIndexed(const std::shared_ptr<VAO>& VAO) override;
	};
}
