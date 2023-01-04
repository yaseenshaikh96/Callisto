#include "CallistoPCH.h"
#include "Renderer.h"

#include <Callisto/Renderer/Renderer2D.h>

#include <Callisto/Platforms/OpenGL/OpenGLShader.h>

namespace Callisto
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData{};

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		dynamic_cast<Callisto::OpenGLShader*>(shader.get())->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		dynamic_cast<Callisto::OpenGLShader*>(shader.get())->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}