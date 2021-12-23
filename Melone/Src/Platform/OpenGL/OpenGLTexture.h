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
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D(void);
	public:
		virtual void bind(unsigned int slot = 0) const override;

		virtual int getWidth(void) const override { return mWidth; };
		virtual int getHeight(void) const override { return mHeight; };

		virtual void setData(void* data, unsigned int size) override;
	};
}

