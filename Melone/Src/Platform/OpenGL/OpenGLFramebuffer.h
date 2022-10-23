#pragma once
#include "Melone/Renderer/Framebuffer.h"

namespace Melone 
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		void Resize(unsigned int width, unsigned int height) override;

		int ReadPixel(unsigned int attachmentIndex, int x, int y) override;

		void ClearAttachment(unsigned int attachmentIndex, int value) override;

		unsigned int GetColorAttachmentRendererID(unsigned int index = 0) const override
		{ 
			MELONE_CORE_ASSERT(index < mColorAttachments.size(), "Invalid index");
			return mColorAttachments[index]; 
		}

		const FramebufferSpecification& GetSpecification() const override { return mSpecification; }
	private:
		unsigned int mRendererID = 0;
		FramebufferSpecification mSpecification;

		std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
		FramebufferTextureSpecification mDepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<unsigned int> mColorAttachments;
		unsigned int mDepthAttachment = 0;
	};
}
