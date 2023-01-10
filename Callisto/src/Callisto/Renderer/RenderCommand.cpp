#include "CallistoPCH.h"
#include "RenderCommand.h"

#include "Callisto/Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Callisto
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}