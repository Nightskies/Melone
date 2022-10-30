#include "mlpch.h"
#include "OpenGlShader.h"

#include "Melone/Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

const char* VERTEX_SHADER_EXTENSION = ".vt";
const char* FRAGMENT_SHADER_EXTENSION = ".fg";

namespace Melone
{
	OpenGLShader::OpenGLShader(std::filesystem::path&& vertShaderPath, std::filesystem::path&& fragShaderPath)
		: 
		mVertShaderPath(std::move(vertShaderPath)),
		mFragShaderPath(std::move(fragShaderPath)),
		mRendererID(0)
	{
		std::string vertShaderSrc = GetSource(mVertShaderPath);
		std::string fragShaderSrc = GetSource(mFragShaderPath);

		unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		unsigned int program = glCreateProgram();

		const char* srcPtr = vertShaderSrc.c_str();
		glShaderSource(vertShader, 1, &srcPtr, nullptr);
		Compile(vertShader);
		glAttachShader(program, vertShader);

		srcPtr = fragShaderSrc.c_str();
		glShaderSource(fragShader, 1, &srcPtr, nullptr);
		Compile(fragShader);
		glAttachShader(program, fragShader);

		Link(program);

		glDetachShader(program, vertShader);
		glDetachShader(program, fragShader);

		mRendererID = program;

		mName = mVertShaderPath.stem().string();
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::GetSource(const std::filesystem::path& shaderPath)
	{
		if (shaderPath.has_extension())
		{
			std::string extension = shaderPath.extension().string();

			if (extension == FRAGMENT_SHADER_EXTENSION ||
				extension == VERTEX_SHADER_EXTENSION)
			{
				std::ifstream file(shaderPath);
				std::stringstream ss;

				if (file)
				{
					ss << file.rdbuf();
				}
				else
				{
					MELONE_CORE_ERROR("File: {0} wasn't open", shaderPath.string());
				}

				return ss.str();
			}
			else
			{
				MELONE_CORE_ERROR("Invalid extension: {0}", extension);
			}
		}
		else
		{
			MELONE_CORE_ERROR("File hasn't extension");
		}
	}

	void OpenGLShader::Compile(unsigned int shader)
	{
		int result;

		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string msg(length, ' ');
			glGetShaderInfoLog(shader, length, &length, msg.data());

			MELONE_CORE_FATAL(msg);

			glDeleteShader(shader);
		}
	}

	void OpenGLShader::Link(unsigned int program)
	{
		int result;

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::string msg(length, ' ');
			glGetProgramInfoLog(program, length, &length, msg.data());

			MELONE_CORE_FATAL(msg);

			glDeleteShader(program);
		}
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
			return mUniformLocationCache[name];

		int location = glGetUniformLocation(mRendererID, name.c_str());

		if (location == -1)
		{
			MELONE_CORE_WARN("Warning: uniform {} doesn't exist", name);
		}
		else
		{
			mUniformLocationCache[name] = location;
		}

		return location;
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, unsigned int count)
	{
		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}
}
