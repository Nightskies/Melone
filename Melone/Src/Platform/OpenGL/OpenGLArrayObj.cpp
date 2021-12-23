#include "mlpch.h"
#include "OpenGLArrayObj.h"

#include <glad/glad.h>

namespace Melone
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
				return GL_FLOAT;
			case ShaderDataType::Float2:
				return GL_FLOAT;
			case ShaderDataType::Float3:
				return GL_FLOAT;
			case ShaderDataType::Float4:
				return GL_FLOAT;
			case ShaderDataType::Mat3:
				return GL_FLOAT;
			case ShaderDataType::Mat4:
				return GL_FLOAT;
			case ShaderDataType::Int:
				return GL_INT;
			case ShaderDataType::Int2:
				return GL_INT;
			case ShaderDataType::Int3:
				return GL_INT;
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::Bool:
				return GL_BOOL;
			default:
				MELONE_CORE_ASSERT(false, "Unknown shader data type");
				return 0;
		}
	}

	OpenGLVAO::OpenGLVAO(void)
	{
		glCreateVertexArrays(1, &mRendererID);
	}

	void OpenGLVAO::bind(void) const
	{
		glBindVertexArray(mRendererID);
	}

	void OpenGLVAO::unbind(void) const
	{
		glBindVertexArray(0);
	}

	void OpenGLVAO::addVBO(const std::shared_ptr<VBO>& VBO)
	{
		MELONE_CORE_ASSERT(VBO->getLayout().getElements().size(), "VBO has no layout");

		glBindVertexArray(mRendererID);
		VBO->bind();

		unsigned int index = 0;
		const auto& layout = VBO->getLayout();

		for (const auto& el : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, el.getComponentCount(), ShaderDataTypeToOpenGLBaseType(el.type),
				el.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)el.offset);
			index++;
		}

		mVBOArr.push_back(VBO);
	}

	void OpenGLVAO::setIBO(const std::shared_ptr<IBO>& IBO)
	{
		glBindVertexArray(mRendererID);
		IBO->bind();

		mIBO = IBO;
	}

	OpenGLVAO::~OpenGLVAO(void)
	{
		glDeleteVertexArrays(1, &mRendererID);
	}
}