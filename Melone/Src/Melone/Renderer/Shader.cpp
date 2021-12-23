#include "mlpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	std::shared_ptr<Shader> Shader::create(const std::string& shaderFilePath)
	{
		return std::make_shared<OpenGLShader>(shaderFilePath);
	}
}