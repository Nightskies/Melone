#pragma once

namespace Melone
{
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext(void) = default;

		virtual void init(void) = 0;
		virtual void swapBuffers(void) = 0;
	};
}