#pragma once

#include "Melone/Renderer/BufferObj.h"

namespace Melone
{
	// Vertex Array Object
	class VAO
	{
	public:
		virtual ~VAO(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual void addVBO(const std::shared_ptr<VBO>& VBO) = 0;
		virtual void setIBO(const std::shared_ptr<IBO>& IBO) = 0;

		static std::shared_ptr<VAO> create(void);
	};
}
