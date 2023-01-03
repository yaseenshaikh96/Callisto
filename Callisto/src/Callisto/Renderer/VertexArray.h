#pragma once
#include "buffer.h"

#include <memory>

namespace Callisto
{
	class VertexArray
	{
	public:
		static Ref<VertexArray> Create();
	public:
		virtual ~VertexArray() = default;
	public:
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		
		virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}