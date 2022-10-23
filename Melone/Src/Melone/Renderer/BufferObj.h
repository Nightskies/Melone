#pragma once

namespace Melone
{
	enum class ShaderDataType
	{
		Undefined,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
				return sizeof(float);
			case ShaderDataType::Float2:
				return sizeof(float) * 2;
			case ShaderDataType::Float3:
				return sizeof(float) * 3;
			case ShaderDataType::Float4:
				return sizeof(float) * 4;
			case ShaderDataType::Mat3:
				return sizeof(float) * 9;
			case ShaderDataType::Mat4:
				return sizeof(float) * 16;
			case ShaderDataType::Int:
				return sizeof(int);
			case ShaderDataType::Int2:
				return sizeof(int) * 2;
			case ShaderDataType::Int3:
				return sizeof(int) * 3;
			case ShaderDataType::Int4:
				return sizeof(int) * 4;
			case ShaderDataType::Bool:
				return sizeof(bool);
			default:
				MELONE_CORE_ASSERT(false, "Unknown shader data type");
				return 0;
		}
	}

	struct VBOElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned int Size;
		unsigned int Offset = 0;
		bool Normalized;

		VBOElement() = default;
		~VBOElement() = default;

		VBOElement(ShaderDataType elType, std::string&& elName, bool elNormalized = false);

		unsigned int GetComponentCount() const;
	};

	class VBOLayout
	{
	public:
		VBOLayout() = default;
		~VBOLayout() = default;

		VBOLayout(std::initializer_list<VBOElement>&& el);

		const std::vector<VBOElement>& GetElements() const { return mElements; }
		unsigned int GetStride() const { return mStride; }

		std::vector<VBOElement>::iterator begin() { return mElements.begin(); }
		std::vector<VBOElement>::iterator end() { return mElements.end(); }

		std::vector<VBOElement>::const_iterator begin() const { return mElements.cbegin(); }
		std::vector<VBOElement>::const_iterator end() const { return mElements.cend(); }
	private:
		void CalculateOffsetAndStride();
	private:
		std::vector<VBOElement> mElements;
		unsigned int mStride = 0;
	};

	// Vertex Buffer Object
	class VBO
	{
	public:
		VBO() = default;
		virtual ~VBO() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, unsigned int size) = 0;

		virtual const VBOLayout& GetLayout() const = 0;
		virtual void SetLayout(VBOLayout&& layout) = 0;

		static SPtr<VBO> Create(unsigned int size);
		static SPtr<VBO> Create(float* vertices, unsigned int size);
	};

	// Index Buffer Object
	class IBO
	{
	public:
		IBO() = default;
		virtual ~IBO() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, unsigned int size) = 0;

		static SPtr<IBO> Create(unsigned int count);
		static SPtr<IBO> Create(unsigned int* indices, unsigned int count);
	};
}