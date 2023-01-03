#pragma once

namespace Callisto
{
	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Callisto::ShaderDataType::Float:		return 4 * 1;
		case Callisto::ShaderDataType::Float2:		return 4 * 2;
		case Callisto::ShaderDataType::Float3:		return 4 * 3;
		case Callisto::ShaderDataType::Float4:		return 4 * 4;
		case Callisto::ShaderDataType::Mat3:		return 4 * 3 * 3;
		case Callisto::ShaderDataType::Mat4:		return 4 * 4 * 4;
		case Callisto::ShaderDataType::Int:			return 4 * 1;
		case Callisto::ShaderDataType::Int2:		return 4 * 2;
		case Callisto::ShaderDataType::Int3:		return 4 * 3;
		case Callisto::ShaderDataType::Int4:		return 4 * 4;
		case Callisto::ShaderDataType::Bool:		return 1;
		}
		CALLISTO_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	class BufferElement
	{
	public:
		BufferElement() {}
	public:
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			:
			Name(name), 
			Type(type), 
			Size(ShaderDataTypeSize(type)), 
			Offset(0), 
			Normalized(normalized)
		{}
	public:
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Callisto::ShaderDataType::Float:	return 1;
			case Callisto::ShaderDataType::Float2:	return 2;
			case Callisto::ShaderDataType::Float3:	return 3;
			case Callisto::ShaderDataType::Float4:	return 4;
			case Callisto::ShaderDataType::Mat3:	return 3 * 3;
			case Callisto::ShaderDataType::Mat4:	return 4 * 4;
			case Callisto::ShaderDataType::Int:		return 1;
			case Callisto::ShaderDataType::Int2:	return 2;
			case Callisto::ShaderDataType::Int3:	return 3;
			case Callisto::ShaderDataType::Int4:	return 4;
			case Callisto::ShaderDataType::Bool:	return 1;
			}
			CALLISTO_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

	public:
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}
	public:
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }


		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

	private:
		void CalculateOffsetAndStride();
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	/**************************************************************************************************************************************************/
	/* VertexBuffer */
	/**************************************************************************************************************************************************/
	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	public:
		virtual ~VertexBuffer() {}
	public:
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	/**************************************************************************************************************************************************/
	/* IndexBuffer */
	/**************************************************************************************************************************************************/
	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	public:
		virtual ~IndexBuffer() {}
	public:
		virtual uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}