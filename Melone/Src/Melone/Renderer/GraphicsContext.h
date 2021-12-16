#pragma once

namespace Melone
{
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = default;

		virtual void init(void) = 0;
		virtual void swapBuffers(void) = 0;
	};
}