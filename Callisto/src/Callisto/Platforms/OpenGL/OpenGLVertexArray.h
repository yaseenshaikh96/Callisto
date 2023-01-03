#pragma once

#include "Callisto/Renderer/VertexArray.h"

namespace Callisto
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
	public:
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
	
		virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
		
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}