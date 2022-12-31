#pragma once

namespace Callisto
{
	class VertexBuffer
	{
	public:
		static VertexBuffer* Create(float* vertices, uint32_t size);
	public:
		virtual ~VertexBuffer() {}
	public:

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	
	};
	class IndexBuffer
	{
	public:
		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	public:
		virtual ~IndexBuffer() {}
	public:
		virtual uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}