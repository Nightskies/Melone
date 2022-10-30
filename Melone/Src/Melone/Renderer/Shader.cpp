#include "mlpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Melone
{
	SPtr<Shader> Shader::Create(std::filesystem::path&& vertShaderPath, std::filesystem::path&& fragShaderPath)
	{
		return std::make_shared<OpenGLShader>(std::move(vertShaderPath), std::move(fragShaderPath));
	}

	void ShaderLibrary::Add(const SPtr<Shader>& shader)
	{
		auto& name = shader->GetName();
		mShaders[name] = shader;
	}

	SPtr<Shader> ShaderLibrary::Load(std::filesystem::path&& vertShaderPath, std::filesystem::path&& fragShaderPath)
	{
		auto shader = Shader::Create(std::move(vertShaderPath), std::move(fragShaderPath));
		Add(shader);
		return shader;
	}

	SPtr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		MELONE_CORE_ASSERT(Exists(name), "Shader not exist");
		return mShaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return mShaders.find(name) != mShaders.end();
	}
}