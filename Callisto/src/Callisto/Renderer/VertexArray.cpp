#include "CallistoPCH.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Callisto/Platforms/OpenGL/OpenGLVertexArray.h"

namespace Callisto
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}