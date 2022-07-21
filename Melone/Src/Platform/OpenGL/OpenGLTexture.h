#pragma once

#include "Melone/Renderer/Texture.h"

using GLenum = unsigned int;

namespace Melone
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		unsigned int mRendererID;
		std::string mPath;
		unsigned int mWidth;
		unsigned int mHeight;
		GLenum mInternalFormat, mDataFormat;
	public:
		OpenGLTexture2D(unsigned int width, unsigned int height);
		OpenGLTexture2D(std::string&& path);
		~OpenGLTexture2D();
	public:
		void Bind(unsigned int slot = 0) const override;

		unsigned int GetWidth() const override { return mWidth; };
		unsigned int GetHeight() const override { return mHeight; };
		unsigned int GetRendererID() const override { return mRendererID; }

		void SetData(void* data, unsigned int size) override;

		bool operator==(const ITexture& other) const override
		{
			return mRendererID == static_cast<const OpenGLTexture2D&>(other).mRendererID;
		}
	};
}

