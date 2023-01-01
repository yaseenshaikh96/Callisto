#include "CallistoPCH.h"
#include "Buffer.h"
#include "Renderer.h"

#include <Callisto/Platforms/OpenGL/OpenGLBuffer.h>

namespace Callisto
{
	void BufferLayout::CalculateOffsetAndStride()
	{
		int32_t offset{ 0 };
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
		}
		m_Stride = offset;
	}
	
	/**************************************************************************************************************************************************/
	/* VertexBuffer */
	/**************************************************************************************************************************************************/
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE: 
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	/**************************************************************************************************************************************************/
	/* IndexBuffer */
	/**************************************************************************************************************************************************/
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}