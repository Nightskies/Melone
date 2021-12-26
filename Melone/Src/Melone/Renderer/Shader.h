#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Melone
{
	class Shader
	{
	public:
		virtual ~Shader(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual void setUniformInt(const std::string& name, int value) = 0;

		virtual void setUniformFloat(const std::string& name, const float value) = 0;
		virtual void setUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void setUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void setUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& getName(void) const = 0;

		static std::shared_ptr<Shader> create(const std::string& shaderFilePath);
	};

	class ShaderLibrary
	{
	public:
		void add(const std::shared_ptr<Shader>& s);
		std::shared_ptr<Shader> load(const std::string& path);

		std::shared_ptr<Shader> get(const std::string& name);

		bool exists(const std::string& name);
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
	};
}