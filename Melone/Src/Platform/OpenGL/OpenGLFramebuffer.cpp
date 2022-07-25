#include "mlpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Melone 
{
	constexpr static unsigned int sMaxFramebufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, unsigned int* outID, unsigned int count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, unsigned int id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(unsigned int id, int samples, GLenum internalFormat, GLenum format, unsigned int width, unsigned int height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(unsigned int id, int samples, GLenum format, GLenum attachmentType, unsigned int width, unsigned int height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8: 
				return true;
			}

			return false;
		}

		static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       
				return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: 
				return GL_RED_INTEGER;
			default:
				MELONE_CORE_WARN("Invalid format");
			}

			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: 
		mSpecification(spec)
	{
		for (auto spec : mSpecification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				mColorAttachmentSpecifications.emplace_back(spec);
			else
				mDepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (mRendererID)
		{
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);

			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		bool multisample = mSpecification.Samples > 1;

		// Attachments
		if (mColorAttachmentSpecifications.size())
		{
			mColorAttachments.resize(mColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, mColorAttachments.data(), mColorAttachments.size());

			for (size_t i = 0; i < mColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, mColorAttachments[i]);
				switch (mColorAttachmentSpecifications[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_RGBA8, GL_RGBA, mSpecification.Width, mSpecification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_R32I, GL_RED_INTEGER, mSpecification.Width, mSpecification.Height, i);
						break;
				}
			}
		}

		if (mDepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &mDepthAttachment, 1);
			Utils::BindTexture(multisample, mDepthAttachment);
			switch (mDepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(mDepthAttachment, mSpecification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mSpecification.Width, mSpecification.Height);
				break;
			}
		}

		if (mColorAttachments.size() > 1)
		{
			MELONE_CORE_ASSERT(mColorAttachments.size() <= 4, "Too more color attachments");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		MELONE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int OpenGLFramebuffer::ReadPixel(unsigned int attachmentIndex, int x, int y)
	{
		MELONE_CORE_ASSERT(attachmentIndex < mColorAttachments.size(), "Invalid index");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(unsigned int attachmentIndex, int value)
	{
		MELONE_CORE_ASSERT(attachmentIndex < mColorAttachments.size(), "Invalid index");

		auto& spec = mColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(mColorAttachments[attachmentIndex], 0,
			Utils::HazelFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mSpecification.Width, mSpecification.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(unsigned int width, unsigned int height)
	{
		if (width == 0 || height == 0 || width > sMaxFramebufferSize || height > sMaxFramebufferSize)
		{
			MELONE_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		mSpecification.Width = width;
		mSpecification.Height = height;

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}
}
