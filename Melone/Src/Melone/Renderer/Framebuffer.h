#pragma once

#include "Melone/Core/Core.h"

namespace Melone
{
	struct FramebufferSpecification
	{
		unsigned int Width, Height;
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer(void) = default;

		virtual void bind(void) = 0;
		virtual void unbind(void) = 0;

		virtual unsigned int getColorAttachmentRendererID(void) const = 0;
		virtual const FramebufferSpecification& getSpecification(void) const = 0;

		static std::shared_ptr<Framebuffer> create(const FramebufferSpecification& spec);
	};
}

