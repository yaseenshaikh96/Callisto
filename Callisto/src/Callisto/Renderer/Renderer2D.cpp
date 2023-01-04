#include "CallistoPCH.h"
#include "Renderer2D.h"

#include <Callisto/Renderer/RenderCommand.h>
#include <Callisto/Renderer/VertexArray.h>
#include <Callisto/Renderer/Shader.h>

#include <glm/gtc/matrix_transform.hpp>

namespace Callisto
{
	class Renderer2DStorage
	{
	public:
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();


		float vertices[] =
		{
			-0.5f, -0.5f,  0.0f,
			 0.5f, -0.5f,  0.0f,
			 0.5f,  0.5f,  0.0f,
			-0.5f,  0.5f,  0.0f
		};
		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

		s_Data->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> SquareVB = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout2 =
		{
			{ShaderDataType::Float3, "a_Position"}
		};
		SquareVB->SetLayout(layout2);
		s_Data->QuadVertexArray->AddVertexBuffer(SquareVB);

		Ref<IndexBuffer> SquareVI = IndexBuffer::Create(indices, sizeof(indices));
		s_Data->QuadVertexArray->SetIndexBuffer(SquareVI);

		s_Data->QuadShader = Shader::Create("./Assets/Shaders/FlatColor.glsl");
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::DrawQuadFilled(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4 color)
	{
		DrawQuadFilled(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
	}
	void Renderer2D::DrawQuadFilled(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4 color)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", color);

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), 
				glm::radians(rotation),
				glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f) , glm::vec3(size.x, size.y, 1.0f));

		s_Data->QuadShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}