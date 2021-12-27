#include "mlpch.h"
#include "BufferObj.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBufferObj.h"

namespace Melone
{
	VBOElement::VBOElement(ShaderDataType elType, std::string elName, bool elNormalized)
		:
		type(elType),
		name(std::move(elName)),
		size(ShaderDataTypeSize(elType)),
		normalized(elNormalized)
	{}

	unsigned int VBOElement::getComponentCount(void) const
	{
		switch (type)
		{
			case Melone::ShaderDataType::Float:
				return 1;
			case Melone::ShaderDataType::Float2:
				return 2;
			case Melone::ShaderDataType::Float3:
				return 3;
			case Melone::ShaderDataType::Float4:
				return 4;
			case Melone::ShaderDataType::Mat3:
				return 9;
			case Melone::ShaderDataType::Mat4:
				return 16;
			case Melone::ShaderDataType::Int:
				return 1;
			case Melone::ShaderDataType::Int2:
				return 2;
			case Melone::ShaderDataType::Int3:
				return 3;
			case Melone::ShaderDataType::Int4:
				return 4;
			case Melone::ShaderDataType::Bool:
				return 1;
			default:
				MELONE_CORE_ASSERT(false, "Unknown shader data type");
				return 0;
		}
	}

	VBOLayout::VBOLayout(std::initializer_list<VBOElement> el)
		:
		mElements(std::move(el))
	{
		calculateOffsetAndStride();
	}

	void VBOLayout::calculateOffsetAndStride(void)
	{
		unsigned int offset = 0;
		mStride = 0;

		for (auto& el : mElements)
		{
			el.offset = offset;
			offset += el.size;
			mStride += el.size;
		}
	}

	std::shared_ptr<VBO> VBO::create(float* vertices, unsigned int size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::Undefined:
				MELONE_CORE_ASSERT(false, "RendererAPI is undefined");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLVBO>(vertices, size);

			default:
				MELONE_CORE_ASSERT(false, "Unknown renderer api");
				return nullptr;
		}
	}

	std::shared_ptr<IBO> IBO::create(unsigned int* indices, unsigned int count)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::Undefined:
				MELONE_CORE_ASSERT(false, "RendererAPI is undefined");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLIBO>(indices, count);

			default:
				MELONE_CORE_ASSERT(false, "Unknown renderer api");
				return nullptr;
		}
	}
}