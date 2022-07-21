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

	OpenGLVAO::OpenGLVAO()
	{
		glCreateVertexArrays(1, &mRendererID);
	}

	void OpenGLVAO::Bind() const
	{
		glBindVertexArray(mRendererID);
	}

	void OpenGLVAO::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVAO::AddVBO(const SPtr<VBO>& VBO)
	{
		MELONE_CORE_ASSERT(VBO->GetLayout().GetElements().size(), "VBO has no layout");

		glBindVertexArray(mRendererID);
		VBO->Bind();

		unsigned int index = 0;
		const auto& layout = VBO->GetLayout();

		for (const auto& el : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, el.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(el.Type),
				el.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)el.Offset);
			index++;
		}

		mVBOArr.push_back(VBO);
	}

	void OpenGLVAO::SetIBO(const SPtr<IBO>& IBO)
	{
		glBindVertexArray(mRendererID);
		IBO->Bind();

		mIBO = IBO;
	}

	OpenGLVAO::~OpenGLVAO()
	{
		glDeleteVertexArrays(1, &mRendererID);
	}
}