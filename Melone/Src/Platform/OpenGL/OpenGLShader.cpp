#include "mlpch.h"
#include "OpenGlShader.h"

#include "Melone/Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Melone
{
	static GLenum ShaderTypeFromString(const std::string_view& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		MELONE_CORE_ASSERT(false, "Unknown shader type");

		return 0;
	}

	OpenGLShader::OpenGLShader(std::string&& filePath)
		: 
		mFilePath(std::move(filePath)), 
		mRendererID(0)
	{
		std::string src = ReadShaderFile(mFilePath);
		auto shaderSrc = GetSourceShaders(src);
		CompileAndLinkShader(shaderSrc);

		// get file name
		auto lastSlash = mFilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = mFilePath.rfind('.');
		auto count = lastDot == std::string::npos ? mFilePath.size() - lastSlash : lastDot - lastSlash;
		mName = mFilePath.substr(lastSlash, count);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::ReadShaderFile(const std::string& path)
	{
		std::string result;
		std::ifstream stream(path, std::ios::in | std::ios::binary);

		if (stream)
		{
			stream.seekg(0, std::ios::end);
			result.resize(stream.tellg());
			stream.seekg(0, std::ios::beg);
			stream.read(&result[0], result.size());
			stream.close();
		}
		else
		{
			MELONE_CORE_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::GetSourceShaders(const std::string_view& src)
	{
		std::unordered_map<GLenum, std::string> shaderSrc;

		std::string_view typeToken = "#type";
		size_t pos = src.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			MELONE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeToken.size() + 1;
			std::string_view type = src.substr(begin, eol - begin);
			MELONE_CORE_ASSERT(ShaderTypeFromString(type), "Invdlid shader type specified");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrc[ShaderTypeFromString(type)] = src.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		return shaderSrc;
	}

	void OpenGLShader::CompileAndLinkShader(const std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs;
		int shaderID = 0;

		for (const auto& kv : shaderSrc)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* s = source.c_str();
			glShaderSource(shader, 1, &s, nullptr);
			glCompileShader(shader);

			int result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				int length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)alloca(length * sizeof(char));
				glGetShaderInfoLog(shader, length, &length, message);

				glDeleteShader(shader);

				MELONE_CORE_ERROR("{0}", message);
				MELONE_CORE_ASSERT(false, "Shader compilation failure");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[shaderID++] = shader;
		}

		glLinkProgram(program);

		int result;
		glGetShaderiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(program, length, &length, message);

			glDeleteShader(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			MELONE_CORE_ERROR("{0}", message);
			MELONE_CORE_ASSERT(false, "Shader link failure");
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		mRendererID = program;
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
