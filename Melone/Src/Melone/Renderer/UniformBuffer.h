#pragma once

#include "Melone/Core/Core.h"

namespace Melone 
{
	class UniformBuffer
	{
	public:
		UniformBuffer() = default;
		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, unsigned int size, unsigned int offset = 0) = 0;

		static SPtr<UniformBuffer> Create(unsigned int size, unsigned int binding);
	};
}