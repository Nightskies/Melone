#include "mlpch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Melone
{
	std::shared_ptr<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
	{
		return std::make_shared<OpenGLTexture2D>(width, height);
	}

	std::shared_ptr<Texture2D> Texture2D::create(const std::string& path)
	{
		return std::make_shared<OpenGLTexture2D>(path);
	}
}
