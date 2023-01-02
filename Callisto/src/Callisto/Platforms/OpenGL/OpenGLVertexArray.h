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
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual void Bind() const override;
		virtual void UnBind() const override;
	
		virtual const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
		
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}