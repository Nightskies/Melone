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

		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(mVertexBufferIndex);
				glVertexAttribPointer(mVertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);

				mVertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(mVertexBufferIndex);
				glVertexAttribIPointer(mVertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				mVertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(mVertexBufferIndex);
					glVertexAttribPointer(mVertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(mVertexBufferIndex, 1);
					mVertexBufferIndex++;
				}
				break;
			}
			default:
				MELONE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
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