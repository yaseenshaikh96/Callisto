#include "CallistoPCH.h"
#include "Buffer.h"
#include "Renderer.h"

#include <Callisto/Platforms/OpenGL/OpenGLBuffer.h>

namespace Callisto
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE: 
			CALLISTO_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		CALLISTO_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE:
			CALLISTO_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}
		CALLISTO_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}