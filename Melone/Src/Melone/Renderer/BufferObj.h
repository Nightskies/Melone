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

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
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

	class VBOElement
	{
	public:
		std::string name;
		ShaderDataType type;
		unsigned int size;
		unsigned int offset = 0;
		bool normalized;
	public:
		VBOElement(void) = default;
		~VBOElement(void) = default;

		VBOElement(ShaderDataType elType, std::string elName, bool elNormalized = false);

		unsigned int getComponentCount(void) const;
	};

	class VBOLayout
	{
	private:
		std::vector<VBOElement> mElements;
		unsigned int mStride = 0;
	public:
		VBOLayout(void) = default;
		~VBOLayout(void) = default;

		VBOLayout(const std::initializer_list<VBOElement> el);

		const std::vector<VBOElement>& getElements(void) const { return mElements; }
		unsigned int getStride(void) const { return mStride; }

		std::vector<VBOElement>::iterator begin(void) { return mElements.begin(); }
		std::vector<VBOElement>::iterator end(void) { return mElements.end(); }

		std::vector<VBOElement>::const_iterator begin(void) const { return mElements.cbegin(); }
		std::vector<VBOElement>::const_iterator end(void) const { return mElements.cend(); }
	private:
		void calculateOffsetAndStride(void);
	};

	// Vertex Buffer Object
	class VBO
	{
	public:
		virtual ~VBO(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual const VBOLayout& getLayout() const = 0;
		virtual void setLayout(const VBOLayout& layout) = 0;

		static VBO* create(float* vertices, unsigned int size);
	};

	// Index Buffer Object
	class IBO
	{
	public:
		IBO(void) = default;
		virtual ~IBO(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual unsigned int getCount(void) const = 0;

		static IBO* create(unsigned int* indices, unsigned int count);
	};
}
