#pragma once
#include "Melone/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Melone
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::filesystem::path&& vertShaderPath, std::filesystem::path&& fragShaderPath);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		const std::string& GetName() const override { return mName; }

		void SetUniformInt(const std::string& name, int value) override;
		void SetUniformIntArray(const std::string& name, int* values, unsigned int count) override;

		void SetUniformFloat(const std::string& name, const float value) override;
		void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
		void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
		void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

		void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;
	private:
		std::string GetSource(const std::filesystem::path& path);
		void Compile(unsigned int shader);
		void Link(unsigned int program);

		int GetUniformLocation(const std::string& name) const;
	private:
		std::filesystem::path mVertShaderPath;
		std::filesystem::path mFragShaderPath;
		unsigned int mRendererID;
		std::string mName;
		mutable std::unordered_map<std::string, int> mUniformLocationCache;
	};
}