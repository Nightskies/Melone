#include "mlpch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Melone
{
	SPtr<Texture2D> Texture2D::Create(unsigned int width, unsigned int height)
	{
		return std::make_shared<OpenGLTexture2D>(width, height);
	}

	SPtr<Texture2D> Texture2D::Create(std::filesystem::path&& path)
	{
		return std::make_shared<OpenGLTexture2D>(std::move(path));
	}
}