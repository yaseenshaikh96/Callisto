#include "CallistoPCH.h"
#include "Shader.h"

#include <Callisto/Renderer/RendererAPI.h>
#include <Callisto/Platforms/OpenGL/OpenGLShader.h>

namespace Callisto
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			CALLISTO_CORE_ASSERT(false, "RenderAPI::NONE not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		CALLISTO_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}