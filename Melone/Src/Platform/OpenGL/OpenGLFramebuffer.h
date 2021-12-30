#pragma once

#include "Melone/Renderer/Framebuffer.h"

namespace Melone 
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer(void);

		void invalidate(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void resize(unsigned int width, unsigned int height) override;

		virtual unsigned int getColorAttachmentRendererID(void) const override { return mColorAttachment; }

		virtual const FramebufferSpecification& getSpecification(void) const override { return mSpecification; }
	private:
		unsigned int mRendererID = 0;
		unsigned int mColorAttachment = 0;
		unsigned int mDepthAttachment = 0;
		FramebufferSpecification mSpecification;
	};
}
