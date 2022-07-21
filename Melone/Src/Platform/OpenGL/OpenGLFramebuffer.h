#pragma once

#include "Melone/Renderer/Framebuffer.h"

namespace Melone 
{
	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		unsigned int mRendererID = 0;
		unsigned int mColorAttachment = 0;
		unsigned int mDepthAttachment = 0;
		FramebufferSpecification mSpecification;
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(unsigned int width, unsigned int height) override;

		virtual unsigned int GetColorAttachmentRendererID() const override { return mColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return mSpecification; }
	};
}
