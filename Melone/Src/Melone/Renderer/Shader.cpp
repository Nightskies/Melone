#include "mlpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	std::shared_ptr<Shader> Shader::create(const std::string& shaderFilePath)
	{
		return std::make_shared<OpenGLShader>(shaderFilePath);
	}

	void ShaderLibrary::add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->getName();
		mShaders[name] = shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::load(const std::string& path)
	{
		auto shader = Shader::create(path);
		add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name)
	{
		MELONE_CORE_ASSERT(exists(name), "Shader not exist");
		return mShaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name)
	{
		return mShaders.find(name) != mShaders.end();
	}
}