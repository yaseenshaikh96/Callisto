#pragma once

#include "Callisto/Renderer/Buffer.h"

namespace Callisto
{
	/**************************************************************************************************************************************************/
	/* VertexBuffer */
	/**************************************************************************************************************************************************/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
	public:
		virtual ~OpenGLVertexBuffer();
	public:
		virtual void Bind() const;
		virtual void UnBind() const;
	private:
		uint32_t m_RendererID;
	};

	/**************************************************************************************************************************************************/
	/* IndexBuffer */
	/**************************************************************************************************************************************************/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t Count);
	public:
		virtual ~OpenGLIndexBuffer();
	public:

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual uint32_t GetCount() const { return m_Count; };

	private:
		uint32_t m_Count;
		uint32_t m_RendererID;
	};
}