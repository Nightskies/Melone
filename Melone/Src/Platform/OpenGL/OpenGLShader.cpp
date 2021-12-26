#include "mlpch.h"
#include "OpenGlShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Melone
{
	static GLenum shaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		MELONE_CORE_ASSERT(false, "Unknown shader type");

		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
		: 
		mFilePath(filePath), 
		mRendererID(0)
	{
		std::string src = readShaderFile(filePath);
		auto shaderSrc = getSourceShaders(src);
		compileAndLinkShader(shaderSrc);

		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		mName = filePath.substr(lastSlash, count);
	}

	void OpenGLShader::bind(void) const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::unbind(void) const
	{
		glUseProgram(0);
	}

	std::string OpenGLShader::readShaderFile(const std::string& path)
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

	std::unordered_map<GLenum, std::string> OpenGLShader::getSourceShaders(const std::string& src)
	{
		std::unordered_map<GLenum, std::string> shaderSrc;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			MELONE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			MELONE_CORE_ASSERT(shaderTypeFromString(type), "Invdlid shader type specified");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrc[shaderTypeFromString(type)] = src.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		return shaderSrc;
	}

	void OpenGLShader::compileAndLinkShader(const std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs;
		int shaderID = 0;

		for (auto& kv : shaderSrc)
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

	int OpenGLShader::getUniformLocation(const std::string& name) const
	{
		if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
			return mUniformLocationCache[name];

		int location = glGetUniformLocation(mRendererID, name.c_str());
		if (location == -1)
			std::cout << "Warning: uniform '" << name << " ' doesn't exist" << std::endl;

		mUniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::setUniformInt(const std::string& name, int value)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformFloat(const std::string& name, float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::setUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::setUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::setUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::setUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	OpenGLShader::~OpenGLShader(void)
	{
		glDeleteProgram(mRendererID);
	}
}
