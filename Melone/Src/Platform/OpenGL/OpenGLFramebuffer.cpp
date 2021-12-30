#include "mlpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Melone 
{

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: 
		mSpecification(spec)
	{
		invalidate();
	}

	void OpenGLFramebuffer::invalidate(void)
	{
		if (mRendererID)
		{
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(1, &mColorAttachment);
			glDeleteTextures(1, &mDepthAttachment);
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		glBindTexture(GL_TEXTURE_2D, mColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mSpecification.Width, mSpecification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
		glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mSpecification.Width, mSpecification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

		MELONE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mSpecification.Width, mSpecification.Height);
	}

	void OpenGLFramebuffer::unbind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(unsigned int width, unsigned int height)
	{
		mSpecification.Width = width;
		mSpecification.Height = height;

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer(void)
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(1, &mColorAttachment);
		glDeleteTextures(1, &mDepthAttachment);
	}
}
