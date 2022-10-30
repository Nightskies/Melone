#pragma once

#include "Melone/Renderer/Texture.h"

using GLenum = unsigned int;

namespace Melone
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(unsigned int width, unsigned int height);
		OpenGLTexture2D(std::filesystem::path&& path);
		~OpenGLTexture2D();

		void Bind(unsigned int slot = 0) const override;

		unsigned int GetWidth() const override { return mWidth; };
		unsigned int GetHeight() const override { return mHeight; };
		unsigned int GetRendererID() const override { return mRendererID; }

		void SetData(void* data, unsigned int size) override;

		bool IsLoaded() const override { return mIsLoaded; }

		bool operator==(const ITexture& other) const override
		{
			return mRendererID == static_cast<const OpenGLTexture2D&>(other).mRendererID;
		}
	private:
		unsigned int mRendererID;
		std::filesystem::path mPath;
		unsigned int mWidth;
		unsigned int mHeight;
		bool mIsLoaded = false;
		GLenum mInternalFormat;
		GLenum mDataFormat;
	};
}

