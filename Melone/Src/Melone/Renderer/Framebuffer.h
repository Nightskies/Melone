#pragma once
#include "Melone/Core/Core.h"

namespace Melone
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		unsigned int Width, Height;
		unsigned int Samples = 1;
		FramebufferAttachmentSpecification Attachments;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(unsigned int width, unsigned int height) = 0;

		virtual unsigned int GetColorAttachmentRendererID(unsigned int index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual int ReadPixel(unsigned int attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(unsigned int attachmentIndex, int value) = 0;

		static SPtr<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}