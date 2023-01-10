#include "CallistoPCH.h"
#include "FrameBuffer.h"

#include "Callisto/Renderer/RendererAPI.h"
#include "Callisto/Platforms/OpenGL/OpenGLFrameBuffer.h"

namespace Callisto
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(specs);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}