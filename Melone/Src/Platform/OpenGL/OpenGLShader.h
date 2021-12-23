#pragma once
#include "Melone/Renderer/Shader.h"
#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace Melone
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader
	{
	private:
		std::string mFilePath;
		uint32_t mRendererID;
		std::string mName;
		mutable std::unordered_map<std::string, int> mUniformLocationCache;
	public:
		OpenGLShader(const std::string& file_path);
		~OpenGLShader(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		virtual const std::string& getName(void) const override { return mName; }

		void setUniformInt(const std::string& name, int value) override;

		void setUniformFloat(const std::string& name, const float value) override;
		void setUniformFloat2(const std::string& name, const glm::vec2& value) override;
		void setUniformFloat3(const std::string& name, const glm::vec3& value) override;
		void setUniformFloat4(const std::string& name, const glm::vec4& value) override;

		void setUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		void setUniformMat4(const std::string& name, const glm::mat4& matrix) override;
	private:
		std::string readShaderFile(const std::string& path);
		std::unordered_map<GLenum, std::string> getSourceShaders(const std::string& src);
		void compileAndLinkShader(const std::unordered_map<GLenum, std::string>& shaderSrc);

		int getUniformLocation(const std::string& name) const;
	};
}


