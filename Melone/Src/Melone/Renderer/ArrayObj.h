#pragma once

#include "Melone/Renderer/BufferObj.h"

namespace Melone
{
	class VAO
	{
	public:
		virtual ~VAO() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVBO(const SPtr<VBO>& VBO) = 0;
		virtual void SetIBO(const SPtr<IBO>& IBO) = 0;

		virtual const SPtr<IBO>& GetIBO() const = 0;

		static SPtr<VAO> Create();
	};
}
